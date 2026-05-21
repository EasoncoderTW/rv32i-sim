.text
.global uart_print

# Custom function to print a string to UART
# void uart_print(const char* str, uint32_t uart_addr);
uart_print:
    # a0: pointer to string (str)
    # a1: address of uart (uart_addr)
uart_print_loop:
    lbu a2, (a0)  # load byte (unsigned) from string into a2
    beq a2, zero, uart_print_end # if a2 is zero (end of string), jump to end
    sb a2, (a1) # store byte from a2 to uart
    addi a0, a0, 1 # increment a0 to point to the next byte in string
    j uart_print_loop
uart_print_end:
    ret