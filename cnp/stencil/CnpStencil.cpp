#include "CnpStencil.h"

#include <cstring>

std::size_t CnpStencil::size() const {
    return code.size();
}

void CnpStencil::patch(
    uint8_t* target,
    std::size_t& target_offset,
    std::initializer_list<const uint32_t*> patches
) const {
    const auto stencil_size = this->size();
    uint8_t* target_addr = &target[target_offset];

    std::memcpy(
        target_addr,
        this->code.data(),
        stencil_size
    );

    auto patch_index = 0;
    for (const auto patch : patches) {
        const auto patch_address = this->patches_addresses[patch_index];
        const auto* data = reinterpret_cast<const uint8_t*>(patch);

        std::memcpy(
            target_addr + patch_address.address,
            data,
            sizeof(uint32_t)
        );

        patch_index++;
    }

    target_offset += stencil_size;
}
