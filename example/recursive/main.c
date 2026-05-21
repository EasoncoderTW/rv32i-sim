#include "uart.h"

int fibonacci_dynamic(int n); // prototype for the assembly function, linear time complexity

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
} // exponential time complexity

int main() {
    int a = 10;
    int r1, r2;

    // r2 = fibonacci(a);
    r2 = fibonacci_dynamic(a);

    UART_PRINTF("fibonacci(%d) = %d\n", a, r2);

    return 0;
}