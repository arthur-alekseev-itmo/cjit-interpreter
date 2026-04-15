#include "CnpStencil.h"

#include <cstring>
#include <iostream>

#include "LIEF/LIEF.hpp"


std::size_t CnpStencil::size() const {
    return code.size();
}

enum PatchStrategy { INDIRECT, DIRECT, INDIRECT_21, INDIRECT_12, DIRECT_26 };

PatchStrategy determine_patch_strategy(uint32_t patch_type) {
#if defined(__x86_64__)
    switch (static_cast<LIEF::ELF::RELOC_x86_64>(patch_type)) {
    case LIEF::ELF::RELOC_x86_64::R_X86_64_GOTPCRELX:
    case LIEF::ELF::RELOC_x86_64::R_X86_64_REX_GOTPCRELX:
    case LIEF::ELF::RELOC_x86_64::R_X86_64_PC32:
        return INDIRECT;
    case LIEF::ELF::RELOC_x86_64::R_X86_64_32S:
        return DIRECT;
    default: throw std::runtime_error("More cases need to be supported!!!");
    }
#elif defined(__aarch64__)
    switch (static_cast<LIEF::MachO::ARM64_RELOCATION>(patch_type)) {
    case LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_GOT_LOAD_PAGE21:
        return INDIRECT_21;
    case LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_GOT_LOAD_PAGEOFF12:
        return INDIRECT_12;
    case LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_PAGE21:
        return INDIRECT_21;
    case LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_PAGEOFF12:
        return INDIRECT_12;
    case LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_BRANCH26:
        return DIRECT_26;
    default: throw std::runtime_error("More cases need to be supported!!!");
    }
#else
    throw std::runtime_error("Cannot determine strategy: unknown platform")
#endif

}

// I took this from CPython (https://github.com/python/cpython/blob/main/Python/jit.c)
static uint32_t get_bits(uint64_t value, uint8_t value_start, uint8_t width) {
    return (value >> value_start) & ((1ULL << width) - 1);
}

// I took this from CPython (https://github.com/python/cpython/blob/main/Python/jit.c)
static void set_bits(uint32_t *loc, uint8_t loc_start, uint64_t value, uint8_t value_start, uint8_t width) {
    uint32_t temp_val;
    memcpy(&temp_val, loc, sizeof(temp_val));
    temp_val &= ~(((1ULL << width) - 1) << loc_start);
    const uint32_t patch = get_bits(value, value_start, width) << loc_start;
    temp_val |= patch;
    memcpy(loc, &temp_val, sizeof(temp_val));
}

static uint32_t resolve_size_aarch64(uint32_t x) {
    if (x < 8) return 8;
    return x;
}

void CnpStencil::patch(
    uint8_t* function_pointer,
    std::size_t& function_offset,
    uint8_t* got_pointer,
    std::size_t& got_offset,
    const std::initializer_list<CnpPatchValue> values
) const {
    const auto stencil_size = this->size();
    uint8_t* target_addr = &function_pointer[function_offset];

    std::memcpy(
        target_addr,
        this->code.data(),
        stencil_size
    );

    std::size_t current_value = 0;
    const auto values_vector = std::vector(values.begin(), values.end());

    for (const auto patch : patches) {
        const auto strategy = determine_patch_strategy(patch.type);

        switch (strategy) {
        case INDIRECT:
            {
                const uint64_t relative_got_offset = (got_pointer + got_offset) - (function_pointer + function_offset + patch.address + sizeof(uint32_t));
                // We copy to Fake GOT, and put its relative address to instruction
                memcpy(
                    got_pointer + got_offset,
                    values_vector[current_value].address,
                    values_vector[current_value].size
                );
                memcpy(
                    function_pointer + function_offset + patch.address,
                    &relative_got_offset,
                    sizeof(uint32_t)
                );
                got_offset += values_vector[current_value].size;
                current_value++;
                break;
            }
        case DIRECT:
            memcpy(
                function_pointer + function_offset + patch.address,
                values_vector[current_value].address,
                sizeof(uint32_t)
            );
            current_value++;
            break;
        case INDIRECT_21:
            {
                const auto got_page = reinterpret_cast<uint64_t>(got_pointer + got_offset) & ~0xFFF;
                const auto ip_page = reinterpret_cast<uint64_t>(function_pointer + function_offset + patch.address + sizeof(uint32_t)) & ~0xFFF;
                const auto relative_got_page = got_page - ip_page;
                memcpy(
                    got_pointer + got_offset,
                    values_vector[current_value].address,
                    values_vector[current_value].size
                );
                auto* location = reinterpret_cast<uint32_t*>(function_pointer + function_offset + patch.address);
                set_bits(location, 29, relative_got_page, 0, 2);
                set_bits(location, 5, relative_got_page, 2, 19);
                break;
            }
        case INDIRECT_12:
            {
                auto* location = reinterpret_cast<uint32_t*>(function_pointer + function_offset + patch.address);
                const auto absolute_got_offset = reinterpret_cast<uint64_t>(got_pointer + got_offset);
                uint32_t instr = *location;
                uint8_t shift = (instr >> 30) & 0x3;
                uint64_t value = absolute_got_offset & 0xFFF;
                set_bits(location, 10, value, shift, 12);
                got_offset += resolve_size_aarch64(values_vector[current_value].size);
                current_value++;
                break;
            }
        case DIRECT_26:
            {
                auto location_ptr = function_pointer + function_offset + patch.address;
                const auto location = reinterpret_cast<uint32_t*>(location_ptr);
                const auto patch_value = *reinterpret_cast<const uint32_t*>(values_vector[current_value].address);
                set_bits(location, 0, patch_value, 2, 26);
                current_value++;
                break;
            }
        }
    }

    function_offset += stencil_size;
}
