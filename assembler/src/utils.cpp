#include "utils.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

string load_asm_file(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filename);
    }

    stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

AsmInstruction parse_asm_instruction(const string &line) {
    AsmInstruction instruction;
    size_t pos = 0;
    size_t end;

    // Check for label
    end = line.find(':', pos);
    if (end != string::npos) {
        instruction.label = line.substr(pos, end - pos);
        pos = end + 1;
    }

    // Skip whitespace
    while (pos < line.size() && isspace(line[pos])) {
        pos++;
    }

    // Get opcode
    end = line.find(' ', pos);
    if (end == string::npos) {
        instruction.opcode = line.substr(pos);
        return instruction;
    }
    instruction.opcode = line.substr(pos, end - pos);
    pos = end + 1;

    // Get operands
    while (pos < line.size()) {
        // skip whitespace
        while (pos < line.size() && isspace(line[pos])) {
            pos++;
        }
        end = line.find(',', pos);
        if (end == string::npos) {
            instruction.operands.push_back(line.substr(pos));
            break;
        }
        instruction.operands.push_back(line.substr(pos, end - pos));
        pos = end + 1;
    }

    return instruction;
}

vector<AsmInstruction> parse_asm_program(const string &program) {
    vector<AsmInstruction> instructions;
    stringstream ss(program);
    string line;

    while (getline(ss, line)) {
        // Remove comments
        size_t comment_pos = line.find('#');
        if (comment_pos != string::npos) {
            line = line.substr(0, comment_pos);
        }

        // Trim whitespace
        size_t first = line.find_first_not_of(" \t");
        if (first == string::npos) continue; // skip empty lines
        size_t last = line.find_last_not_of(" \t");
        line = line.substr(first, last - first + 1);

        if (!line.empty()) {
            instructions.push_back(parse_asm_instruction(line));
        }
    }

    return instructions;
}

void print_asm_instruction(const AsmInstruction &instruction) {
    if (!instruction.label.empty()) {
        cout << instruction.label << ": ";
    }
    cout << instruction.opcode;
    for (size_t i = 0; i < instruction.operands.size(); i++) {
        if (i == 0) {
            cout << " " << instruction.operands[i];
        } else {
            cout << ", " << instruction.operands[i];
        }
    }
    cout << endl;
}