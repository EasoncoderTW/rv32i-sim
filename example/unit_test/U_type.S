.text
# U-type Instruction Examples
main:
    lui x5, 0x12345      # Load upper immediate into x5
    auipc x6, 0x6789     # Add upper immediate to PC and store in x6
label:
    ecall                # ecall to exit