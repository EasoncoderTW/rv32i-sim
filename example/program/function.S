.text
main:
    addi a0, zero, 5 # a = 5
    addi a1, zero, 8 # b = 8
    jal ra, mul_func # call mul_func(a, b)
    # x15 now contains the result of a * b (5 * 8 = 40)
    add t1, a0, zero # move result to x10 for further use
    ecall

# function mul_func(a, b):
# param: a0 = a, a1 = b
# return: a0 = a * b
mul_func:
    add x14, zero, zero # x14 = 0

add_loop:
    add x14, x14, a0 # x14 += a
    addi a1, a1, -1 # b -= 1
    bne a1, zero, add_loop # if b != 0, repeat loop

    add a0, x14, zero # a0 = x14 (result)
    jalr zero, 0(ra) # return to caller