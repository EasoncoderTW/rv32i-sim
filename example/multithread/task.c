#include "task.h"
#include "uart.h"

#include <stdint.h>

#define MTIME_OFFSET 0xBFF8
#define MTIME_OFFSET_H 0xBFFC
#define MTIMECMP_OFFSET 0x4000
#define MTIMECMP_OFFSET_H 0x4004
#define MTIME_BASE 0x20000000

#define MTIMECMP_ADDR (MTIME_BASE + MTIMECMP_OFFSET)
#define MTIME_ADDR (MTIME_BASE + MTIME_OFFSET)
#define MTIMECMP_ADDR_H (MTIME_BASE + MTIMECMP_OFFSET_H)
#define MTIME_ADDR_H (MTIME_BASE + MTIME_OFFSET_H)

enum {
	CONTEXT_RA = 0,
	CONTEXT_GP,
	CONTEXT_TP,
	CONTEXT_T0,
	CONTEXT_T1,
	CONTEXT_T2,
	CONTEXT_S0,
	CONTEXT_S1,
	CONTEXT_A0,
	CONTEXT_A1,
	CONTEXT_A2,
	CONTEXT_A3,
	CONTEXT_A4,
	CONTEXT_A5,
	CONTEXT_A6,
	CONTEXT_A7,
	CONTEXT_S2,
	CONTEXT_S3,
	CONTEXT_S4,
	CONTEXT_S5,
	CONTEXT_S6,
	CONTEXT_S7,
	CONTEXT_S8,
	CONTEXT_S9,
	CONTEXT_S10,
	CONTEXT_S11,
	CONTEXT_T3,
	CONTEXT_T4,
	CONTEXT_T5,
	CONTEXT_T6,
	CONTEXT_MEPC,
	CONTEXT_RESERVED,
	CONTEXT_WORDS,
};

typedef struct {
	uint32_t *stack_pointer;
	const char *name;
	uint32_t switches;
} TaskControlBlock;

static TaskControlBlock tasks[TASK_MAX_TASKS];
static uint32_t task_count = 0;
static uint32_t current_task = 0;
static uint32_t scheduler_ticks = 0;
static uint32_t time_slice_ticks = 1;

static uint64_t timer_read_mtime(void) {
	volatile uint32_t *mtime = (volatile uint32_t *)MTIME_ADDR;
	volatile uint32_t *mtime_h = (volatile uint32_t *)MTIME_ADDR_H;
	uint32_t hi_before = 0;
	uint32_t lo = 0;
	uint32_t hi_after = 0;

	do {
		hi_before = *mtime_h;
		lo = *mtime;
		hi_after = *mtime_h;
	} while (hi_before != hi_after);

	return ((uint64_t)hi_after << 32) | lo;
}

static void timer_write_mtimecmp(uint64_t value) {
	volatile uint32_t *mtimecmp = (volatile uint32_t *)MTIMECMP_ADDR;
	volatile uint32_t *mtimecmp_h = (volatile uint32_t *)MTIMECMP_ADDR_H;

	*mtimecmp_h = 0xFFFFFFFFu;
	*mtimecmp = (uint32_t)(value & 0xFFFFFFFFu);
	*mtimecmp_h = (uint32_t)(value >> 32);
}

static void timer_arm_next_slice(void) {
	timer_write_mtimecmp(timer_read_mtime() + time_slice_ticks);
}

static void disable_scheduler_interrupts(void) {
	__asm__ volatile (
		"li t0, 0x80\n"
		"csrc mie, t0\n"
		"li t0, 0x8\n"
		"csrc mstatus, t0\n"
		:
		:
		: "t0", "memory"
	);
}

void task_system_init(uint32_t requested_time_slice_ticks) {
	uint32_t index = 0;

	task_count = 0;
	current_task = 0;
	scheduler_ticks = 0;
	time_slice_ticks = requested_time_slice_ticks == 0 ? 1u : requested_time_slice_ticks;

	for (index = 0; index < TASK_MAX_TASKS; ++index) {
		tasks[index].stack_pointer = 0;
		tasks[index].name = 0;
		tasks[index].switches = 0;
	}
}

int task_create(const char *name, task_entry_t entry, void *arg, uint32_t *stack_buffer, uint32_t stack_words) {
	uintptr_t stack_top = 0;
	uint32_t *frame = 0;
	uint32_t index = 0;

	if (task_count >= TASK_MAX_TASKS || entry == 0 || stack_buffer == 0 || stack_words <= CONTEXT_WORDS) {
		return -1;
	}

	stack_top = ((uintptr_t)(stack_buffer + stack_words)) & ~(uintptr_t)0xF;
	frame = (uint32_t *)stack_top - CONTEXT_WORDS;
	if (frame < stack_buffer) {
		return -1;
	}

	for (index = 0; index < CONTEXT_WORDS; ++index) {
		frame[index] = 0;
	}

	frame[CONTEXT_A0] = (uint32_t)(uintptr_t)entry;
	frame[CONTEXT_A1] = (uint32_t)(uintptr_t)arg;
	frame[CONTEXT_MEPC] = (uint32_t)(uintptr_t)task_bootstrap;

	tasks[task_count].stack_pointer = frame;
	tasks[task_count].name = name;
	tasks[task_count].switches = 0;
	task_count++;

	return (int)(task_count - 1u);
}

uint32_t *task_schedule_from_interrupt(uint32_t *current_sp) {
	timer_arm_next_slice();
	scheduler_ticks++;

	if (task_count == 0) {
		return current_sp;
	}

	tasks[current_task].stack_pointer = current_sp;
	tasks[current_task].switches++;

	if (task_count == 1) {
		return tasks[current_task].stack_pointer;
	}

	current_task = (current_task + 1u) % task_count;
	return tasks[current_task].stack_pointer;
}

void task_start_scheduler(void) {
	if (task_count == 0) {
		UART_PRINT("No tasks to schedule.\n");
		task_shutdown();
	}

	current_task = 0;
	timer_arm_next_slice();

	__asm__ volatile (
		"li t0, 0x80\n"
		"csrs mie, t0\n"
		"li t0, 0x88\n"
		"csrs mstatus, t0\n"
		:
		:
		: "t0", "memory"
	);

	task_restore_first(tasks[current_task].stack_pointer);
}

void task_bootstrap(task_entry_t entry, void *arg) {
	entry(arg);
	UART_PRINT("\nTask returned unexpectedly.\n");
	task_shutdown();
}

void task_shutdown(void) {
	disable_scheduler_interrupts();
	UART_PRINT("\nScheduler stopped.\n");
	__asm__ volatile ("ecall");

	while (1) {
	}
}

uint32_t task_tick_count(void) {
	return scheduler_ticks;
}

uint32_t task_current_index(void) {
	return current_task;
}

const char *task_current_name(void) {
	if (task_count == 0 || tasks[current_task].name == 0) {
		return "none";
	}

	return tasks[current_task].name;
}
