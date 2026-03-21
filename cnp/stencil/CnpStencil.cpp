#include "CnpStencil.h"

#include <cstring>

std::size_t CnpStencil::size() const {
    return code.size();
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
        const uint32_t relative_got_offset = (got_pointer + got_offset) - (function_pointer + function_offset + patch.address + sizeof(uint32_t));
        switch (patch.type) {
        case LIEF::ELF::RELOC_x86_64::R_X86_64_GOTPCRELX:
        case LIEF::ELF::RELOC_x86_64::R_X86_64_REX_GOTPCRELX:
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
        // TODO: I am really unsure, now just hack for jump
        case LIEF::ELF::RELOC_x86_64::R_X86_64_32S:
            memcpy(
                function_pointer + function_offset + patch.address,
                values_vector[current_value].address,
                sizeof(uint32_t)
            );
            current_value++;
            break;
        default: throw std::runtime_error("More cases need to be supported!!!");
        }
    }

    function_offset += stencil_size;
}
