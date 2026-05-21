#include "uart.h"

// prototype for the assembly function
int gcd_asm(int a, int b); // 16~26 cycles

int gcd_c(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
} // 68 cycles

int main() {
    int a = 25;
    int b = 100;
    int r1, r2;

    r1 = gcd_asm(a, b);
    r2 = gcd_c(a, b);

    UART_PRINTF("gcd_asm(%d, %d) = %d\n", a, b, r1);
    UART_PRINTF("gcd_c(%d, %d) = %d\n", a, b, r2);

    return 0;
}