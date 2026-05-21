#include "uart.h"

#define MTIME_OFFSET 0xBFF8
#define MTIME_OFFSET_H 0xBFFC
#define MTIMECMP_OFFSET 0x4000
#define MTIMECMP_OFFSET_H 0x4004
#define MTIME_BASE 0x20000000

#define MTIMECMP_ADDR (MTIME_BASE + MTIMECMP_OFFSET)
#define MTIME_ADDR (MTIME_BASE + MTIME_OFFSET)
#define MTIMECMP_ADDR_H (MTIME_BASE + MTIMECMP_OFFSET_H)
#define MTIME_ADDR_H (MTIME_BASE + MTIME_OFFSET_H)

static unsigned int timer_count = 0;

void init_timer() {
    // use riscv asm to set up a timer interrupt that increments timer_count every second
    volatile unsigned   int *mtime = (unsigned   int *)MTIME_ADDR;
    volatile unsigned   int *mtimecmp = (unsigned   int *)MTIMECMP_ADDR;
    volatile unsigned   int *mtime_h = (unsigned   int *)MTIME_ADDR_H;
    volatile unsigned   int *mtimecmp_h = (unsigned   int *)MTIMECMP_ADDR_H;

    *mtime = 0; // reset timer
    *mtimecmp = 10000; // set timer to trigger after 1 second (assuming 10kHz timer frequency)
    *mtime_h = 0; // reset upper 32 bits of timer
    *mtimecmp_h = 0; // reset upper 32 bits of timer compare

    // set the timer interrupt handler
    __asm__ volatile (
        "la t0, interrupt_handler\n"
        "csrw mtvec, t0\n"
    );

    // enable timer interrupt (mie, mstatus)
     __asm__ volatile (
        "li t0, 0x80\n" // MIE.MTIE = 1
        "csrs mie, t0\n"
        "li t0, 0x8\n" // MSTATUS.MIE = 1
        "csrs mstatus, t0\n"
    );
}

void software_timer_interrupt_handler() {
    timer_count++;
    volatile unsigned   int *mtime = (unsigned   int *)MTIME_ADDR;
    volatile unsigned   int *mtimecmp = (unsigned   int *)MTIMECMP_ADDR;
    volatile unsigned   int *mtime_h = (unsigned   int *)MTIME_ADDR_H;
    volatile unsigned   int *mtimecmp_h = (unsigned   int *)MTIMECMP_ADDR_H;
    *mtimecmp = *mtime + 10000; // set timer to trigger after 1 second
    *mtimecmp_h = *mtime_h; // set upper 32 bits of timer compare

    UART_PRINT("@\n");
}


int main() {
    UART_PRINT("Timer example\n");

    init_timer();

    timer_count = 0;
    while (1) {
        if (timer_count >= 5) {
            UART_PRINTF("Timer count: %u\n", timer_count);
            break;
        }

        for(volatile int i = 0; i < 100; i++); // busy wait
        UART_PRINT(".");
    }

    UART_PRINT("Timer example finished\n");
    return 0;
}