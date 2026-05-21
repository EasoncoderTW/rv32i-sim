#ifndef UART_H
#define UART_H

#include <stdio.h>
#define UART_PRINT(...) \
    do { \
        uart_print(__VA_ARGS__, UART_ADDR); \
    } while (0)

#define UART_PRINTF(fmt, ...) \
    do { \
        char buffer[256]; \
        snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__); \
        uart_print(buffer, UART_ADDR); \
    } while (0)

#define UART_ADDR 0x400

extern void uart_print(const char *str, unsigned int uart_addr);

#endif // UART_H