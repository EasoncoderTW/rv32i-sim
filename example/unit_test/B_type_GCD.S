.text
# GCD Calculation using B-type Instructions
main:
    addi x2, x0, 18 # a(x2)=18
    addi x3, x0, 12 # b(x3)=12
    blt x2, x3, skip_swap
sort_and_swap:
    add x4, x2, x0  # temp = a
    add x2, x3, x0  # a = b
    add x3, x4, x0  # b = temp
skip_swap:
    sub x3, x3, x2  # b = b - a
    beq x3, x0, end_gcd  # if b == 0, end
    bge x3, x2, skip_swap # if b >= a, repeat
    beq x0, x0, sort_and_swap  # swaps
end_gcd:
    ecall  # ecall to exit
