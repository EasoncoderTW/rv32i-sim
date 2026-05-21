#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include "utils.hpp"
#include "rv32i-constants.hpp"

inline uint32_t sign_extend(uint32_t value, int bits) {
    if (value & (1 << (bits - 1))) {
        return value | ~((1 << bits) - 1);
    }
    return value;
}

inline uint32_t get_opcode(uint32_t instruction) {
    return instruction & 0x7F; // opcode is in bits [6:0]
}

inline uint32_t get_rd(uint32_t instruction) {
    return (instruction >> 7) & 0x1F; // rd is in bits [11:7]
}

inline uint32_t get_funct3(uint32_t instruction) {
    return (instruction >> 12) & 0x7; // funct3 is in bits [14:12]
}

inline uint32_t get_funct7(uint32_t instruction) {
    return (instruction >> 25) & 0x7F; // funct7 is in bits [31:25]
}

inline uint32_t get_rs1(uint32_t instruction) {
    return (instruction >> 15) & 0x1F; // rs1 is in bits [19:15]
}

inline uint32_t get_rs2(uint32_t instruction) {
    return (instruction >> 20) & 0x1F; // rs2 is in bits [24:20]
}

inline uint32_t get_csr(uint32_t instruction) {
    return (instruction >> 20) & 0xFFF; // csr address is in bits [31:20]
}

// Immediate extraction functions for different instruction types
// I-type: imm[31:20]
// S-type: imm[31:25|11:7]
// B-type: imm[31|7|30:25|11:8]
// J-type: imm[31|19:12|20|30:21]
// LUI/AUIPC: imm[31:12]
uint32_t get_imm_i(uint32_t instruction) {
    return sign_extend(instruction >> 20, 12); // imm[31:20] for I-type
}

inline uint32_t get_imm_s(uint32_t instruction) {
    uint32_t imm11_5 = (instruction >> 25) & 0x7F; // imm[11:5] in bits [31:25]
    uint32_t imm4_0 = (instruction >> 7) & 0x1F; // imm[4:0] in bits [11:7]
    return sign_extend((imm11_5 << 5) | imm4_0, 12); // combine and sign-extend
}

inline uint32_t get_imm_b(uint32_t instruction) {
    uint32_t imm12 = (instruction >> 31) & 0x1; // imm[12] in bit [31]
    uint32_t imm10_5 = (instruction >> 25) & 0x3F; // imm[10:5] in bits [30:25]
    uint32_t imm4_1 = (instruction >> 8) & 0xF; // imm[4:1] in bits [11:8]
    uint32_t imm11 = (instruction >> 7) & 0x1; // imm[11] in bit [7]
    return sign_extend((imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1), 13); // combine and sign-extend
}

inline uint32_t get_imm_j(uint32_t instruction) {
    uint32_t imm20 = (instruction >> 31) & 0x1; // imm[20] in bit [31]
    uint32_t imm10_1 = (instruction >> 21) & 0x3FF; // imm[10:1] in bits [30:21]
    uint32_t imm11 = (instruction >> 20) & 0x1; // imm[11] in bit [20]
    uint32_t imm19_12 = (instruction >> 12) & 0xFF; // imm[19:12] in bits [19:12]
    return sign_extend((imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1), 21); // combine and sign-extend
}

inline uint32_t get_imm_u(uint32_t instruction) {
    return instruction & 0xFFFFF000; // imm[31:12] for U-type
}

inline uint32_t get_imm(uint32_t instruction) {
    uint32_t opcode = get_opcode(instruction);
    switch (opcode) {
        case OPCODE_LOAD: // Load (I-type)
        case OPCODE_I_TYPE: // Immediate ALU (I-type)
        case OPCODE_JALR: // JALR (I-type)
            return get_imm_i(instruction);
        case OPCODE_STORE: // Store (S-type)
            return get_imm_s(instruction);
        case OPCODE_BRANCH: // Branch (B-type)
            return get_imm_b(instruction);
        case OPCODE_JAL: // JAL (J-type)
            return get_imm_j(instruction);
        case OPCODE_LUI: // LUI (U-type)
        case OPCODE_AUIPC: // AUIPC (U-type)
            return get_imm_u(instruction);
        default:
            return 0; // R-type and other instructions have no immediate
    }
}

inline uint32_t get_shamt(uint32_t instruction) {
    return (instruction >> 20) & 0x1F; // shamt is in bits [24:20] for shift instructions
}

Instruction decode_instruction(uint32_t instruction_word) {
    Instruction instr = {};
    instr.instruction_word = instruction_word;
    uint32_t opcode = get_opcode(instruction_word);
    uint32_t funct3 = get_funct3(instruction_word);
    instr.opcode = opcode;
    instr.rd = get_rd(instruction_word);
    instr.rs1 = get_rs1(instruction_word);
    instr.rs2 = get_rs2(instruction_word);
    instr.imm = get_imm(instruction_word);
    instr.shamt = get_shamt(instruction_word);
    instr.csr = get_csr(instruction_word);
    // Determine operation based on opcode and funct3

    uint32_t funct7 = get_funct7(instruction_word);
    switch (opcode) {
        case OPCODE_LOAD:
            switch (funct3) {
                case FUNCT3_LB:
                    instr.operation = OPERATION::LB; // Load Byte
                    break;
                case FUNCT3_LH:
                    instr.operation = OPERATION::LH; // Load Halfword
                    break;
                case FUNCT3_LW:
                    instr.operation = OPERATION::LW; // Load Word
                    break;
                case FUNCT3_LBU:
                    instr.operation = OPERATION::LBU; // Load Byte Unsigned
                    break;
                case FUNCT3_LHU:
                    instr.operation = OPERATION::LHU; // Load Halfword Unsigned
                    break;
                default:
                    instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct3
            }
            break;
        case OPCODE_R_TYPE:
            switch (funct3) {
                case FUNCT3_ADD_SUB:
                    if (funct7 == FUNCT7_ADD) {
                        instr.operation = OPERATION::ADD; // ADD
                    } else if (funct7 == FUNCT7_SUB) {
                        instr.operation = OPERATION::SUB; // SUB
                    } else {
                        instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct7
                    }
                    break;
                case FUNCT3_SLL:
                    instr.operation = OPERATION::SLL; // Shift Left Logical
                    break;
                case FUNCT3_SLT:
                    instr.operation = OPERATION::SLT; // Set Less Than
                    break;
                case FUNCT3_SLTU:
                    instr.operation = OPERATION::SLTU; // Set Less Than Unsigned
                    break;
                case FUNCT3_XOR:
                    instr.operation = OPERATION::XOR; // Exclusive OR
                    break;
                case FUNCT3_SRL_SRA:
                    if (funct7 == FUNCT7_SRL) {
                        instr.operation = OPERATION::SRL; // Shift Right Logical
                    } else if (funct7 == FUNCT7_SRA) {
                        instr.operation = OPERATION::SRA; // Shift Right Arithmetic
                    } else {
                        instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct7
                    }
                    break;
                case FUNCT3_OR:
                    instr.operation = OPERATION::OR; // OR
                    break;
                case FUNCT3_AND:
                    instr.operation = OPERATION::AND; // AND
                    break;
                default:
                    instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct3
            }
            break;
        case OPCODE_I_TYPE:
            switch (funct3) {
                case FUNCT3_ADDI:
                    instr.operation = OPERATION::ADDI;
                    break;
                case FUNCT3_SLTI:
                    instr.operation = OPERATION::SLTI;
                    break;
                case FUNCT3_SLTIU:
                    instr.operation = OPERATION::SLTIU;
                    break;
                case FUNCT3_XORI:
                    instr.operation = OPERATION::XORI;
                    break;
                case FUNCT3_ORI:
                    instr.operation = OPERATION::ORI;
                    break;
                case FUNCT3_ANDI:
                    instr.operation = OPERATION::ANDI;
                    break;
                case FUNCT3_SLLI:
                    instr.operation = OPERATION::SLLI;
                    break;
                case FUNCT3_SRLI_SRAI:
                    if (funct7 == FUNCT7_SRLI) {
                        instr.operation = OPERATION::SRLI;
                    } else if (funct7 == FUNCT7_SRAI) {
                        instr.operation = OPERATION::SRAI;
                    } else {
                        instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct7
                    }
                    break;
                default:
                    instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct3
            }
            break;
        case OPCODE_STORE:
            switch (funct3) {
                case FUNCT3_SB:
                    instr.operation = OPERATION::SB; // Store Byte
                    break;
                case FUNCT3_SH:
                    instr.operation = OPERATION::SH; // Store Halfword
                    break;
                case FUNCT3_SW:
                    instr.operation = OPERATION::SW; // Store Word
                    break;
                default:
                    instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct3
            }
            break;
        case OPCODE_BRANCH:
            switch (funct3) {
                case FUNCT3_BEQ:
                    instr.operation = OPERATION::BEQ; // Branch if equal
                    break;
                case FUNCT3_BNE:
                    instr.operation = OPERATION::BNE; // Branch if not equal
                    break;
                case FUNCT3_BLT:
                    instr.operation = OPERATION::BLT; // Branch if less than
                    break;
                case FUNCT3_BGE:
                    instr.operation = OPERATION::BGE; // Branch if greater than or equal
                    break;
                case FUNCT3_BLTU:
                    instr.operation = OPERATION::BLTU; // Branch if less than unsigned
                    break;
                case FUNCT3_BGEU:
                    instr.operation = OPERATION::BGEU; // Branch if greater than or equal unsigned
                    break;
                default:
                    instr.operation = OPERATION::UNKNOWN; // Default operation for unknown funct3
            }
            break;
        case OPCODE_JAL:
            instr.operation = OPERATION::JAL; // Jump and Link
            break;
        case OPCODE_JALR:
            instr.operation = OPERATION::JALR; // Jump and Link Register
            break;
        case OPCODE_LUI:
            instr.operation = OPERATION::LUI; // Load Upper Immediate
            break;
        case OPCODE_AUIPC:
            instr.operation = OPERATION::AUIPC; // Add Upper Immediate to PC
            break;
        case OPCODE_SYSTEM:
            if (instruction_word == ECALL_OPCODE) {
                instr.operation = OPERATION::ECALL; //
            } else if (instruction_word == MRET_OPCODE) {
                instr.operation = OPERATION::MRET;
            } else {
                switch (funct3) {
                    case FUNCT3_CSRRW:
                        instr.operation = OPERATION::CSRRW;
                        break;
                    case FUNCT3_CSRRS:
                        instr.operation = OPERATION::CSRRS;
                        break;
                    case FUNCT3_CSRRC:
                        instr.operation = OPERATION::CSRRC;
                        break;
                    case FUNCT3_CSRRWI:
                        instr.operation = OPERATION::CSRRWI;
                        break;
                    case FUNCT3_CSRRSI:
                        instr.operation = OPERATION::CSRRSI;
                        break;
                    case FUNCT3_CSRRCI:
                        instr.operation = OPERATION::CSRRCI;
                        break;
                    default:
                        instr.operation = OPERATION::UNKNOWN; // Default operation for unknown system instruction
                        break;
                }
            }
            break;
        default: // Default operation for unknown opcode
            instr.operation = OPERATION::UNKNOWN;
    }


    return instr;
}

// Memory size convertion (e.g., 128K -> 131072)
size_t parse_memory_size(const string &size_str) {
    size_t multiplier = 1;
    char suffix = size_str.back();
    string number_part = size_str.substr(0, size_str.size() - 1);

    if (suffix == 'K' || suffix == 'k') {
        multiplier = 1024;
    } else if (suffix == 'M' || suffix == 'm') {
        multiplier = 1024 * 1024;
    } else if (suffix == 'G' || suffix == 'g') {
        multiplier = 1024 * 1024 * 1024;
    } else {
        number_part = size_str; // No suffix, treat entire string as number
    }

    return stoul(number_part) * multiplier;
}