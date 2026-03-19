# RV32I ISA

## RV32I Instructions and Encoding Formats

### R-Type Instructions
| Instruction | Opcode  | Funct3 | Funct7 | Description          |
|-------------|---------|--------|--------|----------------------|
| ADD         | 0110011 | 000    | 0000000| Add                 |
| SUB         | 0110011 | 000    | 0100000| Subtract            |
| SLL         | 0110011 | 001    | 0000000| Shift Left Logical  |
| SLT         | 0110011 | 010    | 0000000| Set Less Than       |
| SLTU        | 0110011 | 011    | 0000000| Set Less Than Unsigned |
| XOR         | 0110011 | 100    | 0000000| XOR                 |
| SRL         | 0110011 | 101    | 0000000| Shift Right Logical |
| SRA         | 0110011 | 101    | 0100000| Shift Right Arithmetic |
| OR          | 0110011 | 110    | 0000000| OR                  |
| AND         | 0110011 | 111    | 0000000| AND                 |

### I-Type Instructions (Immediate Arithmetic and Load)
| Instruction | Opcode  | Funct3 | Description          |
|-------------|---------|--------|----------------------|
| ADDI        | 0010011 | 000    | Add Immediate        |
| SLTI        | 0010011 | 010    | Set Less Than Immediate |
| SLTIU       | 0010011 | 011    | Set Less Than Immediate Unsigned |
| XORI        | 0010011 | 100    | XOR Immediate        |
| ORI         | 0010011 | 110    | OR Immediate         |
| ANDI        | 0010011 | 111    | AND Immediate        |
| SLLI        | 0010011 | 001    | Shift Left Logical Immediate |
| SRLI        | 0010011 | 101    | Shift Right Logical Immediate |
| SRAI        | 0010011 | 101    | Shift Right Arithmetic Immediate |
| LB          | 0000011 | 000    | Load Byte            |
| LH          | 0000011 | 001    | Load Halfword        |
| LW          | 0000011 | 010    | Load Word            |
| LBU         | 0000011 | 100    | Load Byte Unsigned   |
| LHU         | 0000011 | 101    | Load Halfword Unsigned |

### S-Type Instructions
| Instruction | Opcode  | Funct3 | Description          |
|-------------|---------|--------|----------------------|
| SB          | 0100011 | 000    | Store Byte           |
| SH          | 0100011 | 001    | Store Halfword       |
| SW          | 0100011 | 010    | Store Word           |

### B-Type Instructions
| Instruction | Opcode  | Funct3 | Description          |
|-------------|---------|--------|----------------------|
| BEQ         | 1100011 | 000    | Branch if Equal      |
| BNE         | 1100011 | 001    | Branch if Not Equal  |
| BLT         | 1100011 | 100    | Branch if Less Than  |
| BGE         | 1100011 | 101    | Branch if Greater or Equal |
| BLTU        | 1100011 | 110    | Branch if Less Than Unsigned |
| BGEU        | 1100011 | 111    | Branch if Greater or Equal Unsigned |

### U-Type Instructions
| Instruction | Opcode  | Description          |
|-------------|---------|----------------------|
| LUI         | 0110111 | Load Upper Immediate |
| AUIPC       | 0010111 | Add Upper Immediate to PC |

### J-Type Instructions
| Instruction | Opcode  | Description          |
|-------------|---------|----------------------|
| JAL         | 1101111 | Jump and Link        |
| JALR        | 1100111 | Jump and Link Register |

### Other Instructions
| Instruction | Opcode  | Funct3 | Description          |
|-------------|---------|--------|----------------------|
| ECALL       | 1110011 | 000    | Environment Call      |
| EBREAK      | 1110011 | 000    | Environment Break     |