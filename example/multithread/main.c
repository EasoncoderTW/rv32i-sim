#include "uart.h"
#include "task.h"

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
    TIME_SLICE_TICKS = 250,
    DEMO_TICK_LIMIT = 40,
    TASK_A_WORK = 48,
    TASK_B_WORK = 72,
};

static uint32_t task_a_stack[TASK_STACK_WORDS];
static uint32_t task_b_stack[TASK_STACK_WORDS];
static volatile uint32_t task_a_started = 0;
static volatile uint32_t task_b_started = 0;

void init_timer(void) {
    volatile uint32_t *mtime = (volatile uint32_t *)MTIME_ADDR;
    volatile uint32_t *mtimecmp = (volatile uint32_t *)MTIMECMP_ADDR;
    volatile uint32_t *mtime_h = (volatile uint32_t *)MTIME_ADDR_H;
    volatile uint32_t *mtimecmp_h = (volatile uint32_t *)MTIMECMP_ADDR_H;

    *mtime = 0;
    *mtime_h = 0;
    *mtimecmp = 0xFFFFFFFFu;
    *mtimecmp_h = 0xFFFFFFFFu;

    __asm__ volatile (
        "la t0, interrupt_handler\n"
        "csrw mtvec, t0\n"
        :
        :
        : "t0", "memory"
    );
}

static void busy_work(uint32_t iterations) {
    volatile uint32_t index = 0;

    for (index = 0; index < iterations; ++index) {
    }
}

static void task_a(void *arg) {
    uint32_t counter = 0;
    (void)arg;

    while (1) {
        task_a_started = 1;
        counter++;
        UART_PRINT("A");

        busy_work(TASK_A_WORK);
        if (task_b_started != 0 && task_tick_count() >= DEMO_TICK_LIMIT && counter >= 6u) {
            task_shutdown();
        }
    }
}

static void task_b(void *arg) {
    (void)arg;

    while (1) {
        task_b_started = 1;
        UART_PRINT("B");

        busy_work(TASK_B_WORK);
    }
}

int main(void) {
    UART_PRINT("Simplified RTOS demo\n");

    task_system_init(TIME_SLICE_TICKS);
    init_timer();

    if (task_create("task-A", task_a, 0, task_a_stack, TASK_STACK_WORDS) < 0) {
        UART_PRINT("Failed to create task A.\n");
        return 1;
    }

    if (task_create("task-B", task_b, 0, task_b_stack, TASK_STACK_WORDS) < 0) {
        UART_PRINT("Failed to create task B.\n");
        return 1;
    }

    UART_PRINT("Starting two round-robin tasks: ");
    task_start_scheduler();
}