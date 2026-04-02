.text
.global gcd_asm

# Custom function to compute the greatest common divisor (GCD) of two numbers
gcd_asm:
    # a0: first number (x)
    # a1: second number (y)
    blt a0, a1, skip_swap
sort_and_swap:
    add t0, a0, zero  # temp = a
    add a0, a1, zero  # a = b
    add a1, t0, zero  # b = temp
skip_swap:
    sub a1, a1, a0  # b = b - a
    beq a1, zero, end_gcd  # if b == 0, end
    bge a1, a0, skip_swap # if b >= a, repeat
    j sort_and_swap  # swaps
end_gcd:
    ret # return a (in a0) as the GCD of the original two numbers