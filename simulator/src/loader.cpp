#include <loader.hpp>
#include <vector>
#include <iostream>

using namespace std;

void ELFLoader::load_elf(const string &file_path) {
    ifstream elf_file(file_path, ios::binary);
    if (!elf_file) {
        throw runtime_error("Failed to open ELF file: " + file_path);
    }

    // Read the entire ELF file into memory
    elf_file.seekg(0, ios::end);
    size_t file_size = elf_file.tellg();
    elf_file.seekg(0, ios::beg);

    elf_data.resize(file_size);
    elf_file.read(reinterpret_cast<char*>(elf_data.data()), file_size);

    // Parse ELF header
    Elf32_Ehdr *ehdr = reinterpret_cast<Elf32_Ehdr*>(elf_data.data());
    if (ehdr->e_ident[0] != 0x7F || ehdr->e_ident[1] != 'E' || ehdr->e_ident[2] != 'L' || ehdr->e_ident[3] != 'F') {
        throw runtime_error("Invalid ELF file: " + file_path);
    }

    entry_point = ehdr->e_entry;

    // Parse program headers
    Elf32_Phdr *phdrs = reinterpret_cast<Elf32_Phdr*>(elf_data.data() + ehdr->e_phoff);
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdrs[i].p_type == PT_LOAD) { // Only consider loadable segments
            program_headers.push_back(phdrs[i]);
            cout << "Find segment: vaddr=0x" << hex << phdrs[i].p_vaddr << ", offset=0x" << phdrs[i].p_offset
                 << ", filesz=0x" << phdrs[i].p_filesz << dec << endl;
        }
    }
}

void ELFLoader::load_program_to_memory(Memory &memory) {
    for (const auto &phdr : program_headers) {
        if (phdr.p_type == PT_LOAD) {
            cout << "Loading segment to memory: vaddr=0x" << hex << phdr.p_vaddr << ", offset=0x" << phdr.p_offset
                 << ", filesz=0x" << phdr.p_filesz << dec << endl;
            // Load segment data into memory at the specified virtual address
            for (size_t offset = 0; offset < phdr.p_filesz; offset++) {
                uint8_t byte = elf_data[phdr.p_offset + offset];
                memory.write(phdr.p_vaddr + offset, byte, 0x1); // Store byte by byte
            }
        }
    }
}