.text
.global fibonacci_dynamic

# a0: n
fibonacci_dynamic:
    # preluge
    addi sp, sp, -4 # Allocate stack space for 5 registers
    sw ra, 4(sp)   # Save return address
    addi t0, a0, -1    # t0 = n-1 (function argument)

    # base case: if n <= 1, return n
    li t0, 1        # t0 = 1
    ble a0, t0, fibonacci_dynamic_end

    li t1, 0        # t1 = 0
    li t2, 1        # t2 = 1

fibonacci_dynamic_loop:
    add t3, t1, t2   # t3 = t1 + t2 (next Fibonacci number)
    mv t1, t2        # t1 = t2 (previous Fibonacci number)
    mv t2, t3        # t2 = t3 (current Fibonacci number)
    addi t0, t0, -1  # t0 = t0 - 1 (decrement count)
    bne t0, x0, fibonacci_dynamic_loop # Loop until count
    mv a0, t2       # a0 = Fibonacci(n)

fibonacci_dynamic_end:
    # epilogue
    lw ra, 4(sp)   # Restore return address
    addi sp, sp, 4 # Deallocate stack space
    ret