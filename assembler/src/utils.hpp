#pragma once

#include <cstdint>
#include <vector>
#include <string>

using namespace std;

string load_asm_file(const string &filename);

class AsmProgram {
public:
    string text;
    string data;

    AsmProgram(const string &text_section, const string &data_section)
        : text(text_section), data(data_section) {}

    AsmProgram() : text(""), data("") {}

    AsmProgram(const string &program) {
        size_t data_pos = program.find(".data");
        if (data_pos != string::npos) {
            text = program.substr(0, data_pos);
            data = program.substr(data_pos);
        } else {
            text = program;
            data = "";
        }
    }
};

struct AsmInstruction {
    string label;
    string opcode;
    vector<string> operands;
};

// for one line of assembly
AsmInstruction parse_asm_instruction(const string &line);
// for whole program
vector<AsmInstruction> parse_asm_program(const string &program);

void print_asm_instruction(const AsmInstruction &instruction);