#include "cpu.hpp"
#include "utils.hpp"
#include "memory.hpp"
#include "loader.hpp"
#include "device.hpp"
#include "systembus.hpp"
#include "uart.hpp"

struct ISSConfiguration {
    string elf_file_path;
    size_t memory_size;
};

class ISS{
public:
    ISS(const ISSConfiguration &config) : config(config) {
        cpu = new RV32ICPU();
        memory = new Memory(0x10000000, config.memory_size); // Memory starts at 0x10000000
        bus = new SystemBus();
        uart = new Uart();
        cpu->bind_device(bus);

        attach_devices();

        // Load program from ELF file
        ELFLoader loader;
        loader.load_elf(config.elf_file_path);
        loader.load_program_to_memory(*memory);
        cpu->set_pc(loader.get_entry_point());
    }

    ~ISS() {
        delete cpu;
        delete memory;
        delete bus;
        delete uart;
    }

    void run() {
        std::cout << "Starting simulation..." << std::endl << std::endl << std::endl;
        cpu->run();
    }

    // heller functions
    void dump_cpu_info(bool hexadecimal) {
        cpu->dump_registers(hexadecimal);
        cpu->dump_instruction_count();

    }

    void dump_memory(size_t start_address, size_t length) {
        for (size_t addr = start_address; addr < start_address + length; addr += 4) {
            uint32_t word = memory->read(addr);
            cout << hex << "0x" << addr << ": 0x" << word << dec << endl;
        }
    }

private:

    void attach_devices() {
        bus->attach_device(0x400, 0x100, uart); // UART at 0x400
        bus->attach_device(0x10000000, config.memory_size, memory); // Memory at 0x10000000

        bus->list_devices();
    }

    ISSConfiguration config;

    RV32ICPU *cpu;
    Memory *memory;
    SystemBus *bus;
    Uart *uart;
};