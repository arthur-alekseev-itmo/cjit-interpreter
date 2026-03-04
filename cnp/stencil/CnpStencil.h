#pragma once

#include <algorithm>
#include <memory>

struct CnpStencil {
    std::vector<uint8_t> code;
    std::vector<std::size_t> patches_addresses;

    [[nodiscard]] std::size_t size() const;

    CnpStencil(
        std::vector<uint8_t> code,
        std::vector<std::size_t> patches_addresses
    ) : code(std::move(code)), patches_addresses(std::move(patches_addresses)) {}

    void patch(
        uint8_t* target,
        std::size_t& target_offset,
        std::initializer_list<const uint32_t*> patches
    ) const;

    CnpStencil() = default;
} ;
