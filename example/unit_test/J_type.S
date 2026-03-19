.text
# J-type Instruction Examples
main:
    jal x1, label        # Jump and link to label, x1 = return address
    ecall                # ecall to exit
label:
    jalr x2, 0(x1)      # Jump and link register, x2 = return address, jump to address in x1
    ecall                # ecall to exit