#include <iostream>
#include "ISS.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    ISSConfiguration config;
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " -M <memory_size> <elf_file_path>" << endl;
        return 1;
    }

    // Simple command-line parsing
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-M" && i + 1 < argc) {
            config.memory_size = parse_memory_size(argv[++i]);
        } else {
            config.elf_file_path = argv[i];
        }
    }

    ISS simulator(config);
    simulator.run();
    simulator.dump_registers(true); // dump registers in hexadecimal

    return 0;
}