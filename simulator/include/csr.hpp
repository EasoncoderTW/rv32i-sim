#pragma once

#include <array>
#include <cstdint>

class CsrFile {
public:
    static constexpr uint16_t MSTATUS = 0x300;
    static constexpr uint16_t MIE = 0x304;
    static constexpr uint16_t MTVEC = 0x305;
    static constexpr uint16_t MEPC = 0x341;
    static constexpr uint16_t MCAUSE = 0x342;
    static constexpr uint16_t MIP = 0x344;

    static constexpr uint32_t MSTATUS_MIE = 1u << 3;
    static constexpr uint32_t MSTATUS_MPIE = 1u << 7;
    static constexpr uint32_t MACHINE_TIMER_INTERRUPT_BIT = 1u << 7;
    static constexpr uint32_t MACHINE_TIMER_INTERRUPT_CAUSE = 0x80000007u;

    CsrFile() {
        reset();
    }

    void reset() {
        values.fill(0);
    }

    bool is_supported(uint16_t address) const {
        switch (address) {
            case MSTATUS:
            case MIE:
            case MTVEC:
            case MEPC:
            case MCAUSE:
            case MIP:
                return true;
            default:
                return false;
        }
    }

    uint32_t read(uint16_t address) const {
        if (!is_supported(address)) {
            std::cout << "Warning: Attempt to read unsupported CSR address 0x" << std::hex << address << std::dec << "\n";
            return 0;
        }
        return values[address];
    }

    void write(uint16_t address, uint32_t value) {
        if (!is_supported(address)) {
            std::cout << "Warning: Attempt to write unsupported CSR address 0x" << std::hex << address << std::dec << "\n";
            return;
        }
        values[address] = sanitize(address, value);
    }

    bool machine_interrupts_enabled() const {
        return (read(MSTATUS) & MSTATUS_MIE) != 0;
    }

    bool machine_timer_interrupt_enabled() const {
        return (read(MIE) & MACHINE_TIMER_INTERRUPT_BIT) != 0;
    }

    void set_timer_interrupt_pending(bool pending) {
        uint32_t mip = read(MIP);

        if (pending) {
            mip |= MACHINE_TIMER_INTERRUPT_BIT;
        } else {
            mip &= ~MACHINE_TIMER_INTERRUPT_BIT;
        }

        write(MIP, mip);
    }

    void enter_machine_trap(uint32_t return_pc, uint32_t cause) {
        write(MEPC, return_pc);
        write(MCAUSE, cause);

        uint32_t mstatus = read(MSTATUS);
        uint32_t next_mstatus = mstatus;

        if ((mstatus & MSTATUS_MIE) != 0) {
            next_mstatus |= MSTATUS_MPIE;
        } else {
            next_mstatus &= ~MSTATUS_MPIE;
        }

        next_mstatus &= ~MSTATUS_MIE;
        write(MSTATUS, next_mstatus);
    }

    uint32_t mtvec_base() const {
        return read(MTVEC) & ~0x3u;
    }

    uint32_t mret_pc() {
        uint32_t mstatus = read(MSTATUS);
        uint32_t next_mstatus = mstatus;

        if ((mstatus & MSTATUS_MPIE) != 0) {
            next_mstatus |= MSTATUS_MIE;
        } else {
            next_mstatus &= ~MSTATUS_MIE;
        }

        next_mstatus |= MSTATUS_MPIE;
        write(MSTATUS, next_mstatus);

        return read(MEPC);
    }

private:
    std::array<uint32_t, 4096> values;

    uint32_t sanitize(uint16_t address, uint32_t value) const {
        switch (address) {
            case MSTATUS:
                return value & (MSTATUS_MIE | MSTATUS_MPIE);
            case MIE:
            case MIP:
                return value & MACHINE_TIMER_INTERRUPT_BIT;
            case MTVEC:
            case MEPC:
                return value & ~0x3u;
            case MCAUSE:
                return value;
            default:
                return 0;
        }
    }
};