#include "utils.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <assembly_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    string asm_code;
    try {
        asm_code = load_asm_file(filename);
        cout << "Loaded assembly code from " << filename << ":\n";
        // cout << asm_code << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    // Example usage of AsmProgram
    AsmProgram program(asm_code);
    cout << "Text Section:\n" << program.text << endl;
    cout << "Data Section:\n" << program.data << endl;

    // Example parsing of instructions
    vector<AsmInstruction> instructions = parse_asm_program(program.text);
    cout << "Parsed Instructions:\n";
    for (const auto &instr : instructions) {
        print_asm_instruction(instr);
    }

    return 0;
}
