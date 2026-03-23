#include "CnpStencil.h"

#include <cstring>


std::size_t CnpStencil::size() const {
    return code.size();
}

enum PatchStrategy { INDIRECT, DIRECT, INDIRECT_21, INDIRECT_12, DIRECT_26 };

PatchStrategy determine_patch_strategy(uint32_t patch_type) {
#if defined(__x86_64__)
    switch (static_cast<LIEF::ELF::RELOC_x86_64>(patch_type)) {
    case LIEF::ELF::RELOC_x86_64::R_X86_64_GOTPCRELX:
    case LIEF::ELF::RELOC_x86_64::R_X86_64_REX_GOTPCRELX:
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
    case LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_BRANCH26:
        return DIRECT_26;
    default: throw std::runtime_error("More cases need to be supported!!!");
    }
#else
    throw std::runtime_error("Cannot determine strategy: unknown platform")
#endif

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

        const auto strategy = determine_patch_strategy(patch.type);

        switch (strategy) {
        case INDIRECT:
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
                memcpy(
                    got_pointer + got_offset,
                    values_vector[current_value].address,
                    values_vector[current_value].size
                );
                auto* location = reinterpret_cast<uint32_t*>(function_pointer + function_offset + patch.address);
                const auto patch_value = relative_got_offset;
                const auto adjusted = (patch_value >> 11) & 0x1FFFFF; // top 21 bits
                *location = *location | adjusted;
                got_offset += values_vector[current_value].size;
                break;
            }
        case INDIRECT_12:
            {
                auto* location = reinterpret_cast<uint32_t*>(function_pointer + function_offset + patch.address);
                const auto patch_value = relative_got_offset;
                const auto adjusted = patch_value & 0xFFF; // bottom 12 bits
                *location = *location | adjusted;
                current_value++;
                break;
            }
        case DIRECT_26:
            {
                auto* location = reinterpret_cast<uint32_t*>(function_pointer + function_offset + patch.address);
                const auto patch_value = *reinterpret_cast<const uint32_t*>(values_vector[current_value].address);
                const auto adjusted = patch_value & 0x3FFFFFF; // bottom 26 bits
                *location = *location | adjusted;
                current_value++;
                break;
            }
        }
    }

    function_offset += stencil_size;
}
