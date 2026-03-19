#pragma once
#include <elf.h>
#include <vector>
#include <string>
#include <fstream>
#include "memory.hpp"

using namespace std;

class ELFLoader {
public:
    ELFLoader() : entry_point(0) {}

    void load_elf(const string &file_path);
    void load_program_to_memory(Memory &memory);
    uint32_t get_entry_point() const {
        return entry_point;
    }
private:
    vector<Elf32_Phdr> program_headers;
    uint32_t entry_point;
    vector<uint8_t> elf_data; // to hold the entire ELF file data
};