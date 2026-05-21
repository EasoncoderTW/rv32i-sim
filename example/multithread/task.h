#ifndef MULTITHREAD_TASK_H
#define MULTITHREAD_TASK_H

#include <stdint.h>

enum {
	TASK_MAX_TASKS = 2,
	TASK_STACK_WORDS = 192,
};

typedef void (*task_entry_t)(void *arg);

void task_system_init(uint32_t time_slice_ticks);
int task_create(const char *name, task_entry_t entry, void *arg, uint32_t *stack_buffer, uint32_t stack_words);
uint32_t *task_schedule_from_interrupt(uint32_t *current_sp);
void task_start_scheduler(void) __attribute__((noreturn));
void task_bootstrap(task_entry_t entry, void *arg) __attribute__((noreturn));
void task_shutdown(void) __attribute__((noreturn));

uint32_t task_tick_count(void);
uint32_t task_current_index(void);
const char *task_current_name(void);

void task_restore_first(uint32_t *stack_pointer) __attribute__((noreturn));

#endif
