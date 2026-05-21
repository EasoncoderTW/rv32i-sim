.text
.global _start
_start:
    la sp, _stack_top # Initialize stack pointer
    jal ra, main # Call main function
    ecall # Exit the program