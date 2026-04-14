#pragma once

#include <algorithm>
#include <utility>
#include <vector>

struct CnpStencilPatch {
    uint32_t type;
    std::size_t address;
    uint32_t architecture;

    CnpStencilPatch(
        const uint32_t type,
        const std::size_t address,
        const uint32_t architecture
    ) : type(type), address(address), architecture(architecture) {}
};

struct CnpPatchValue {
    std::size_t size;
    const std::uint8_t* address;

    CnpPatchValue(
        const std::size_t size, const std::uint8_t* address
    ) : size(size), address(address) {}
};


struct CnpStencil {
    std::string name;
    std::vector<uint8_t> code;
    std::vector<CnpStencilPatch> patches;

    [[nodiscard]] std::size_t size() const;

    CnpStencil(
        std::string name,
        std::vector<uint8_t> code,
        std::vector<CnpStencilPatch> patches
    ) : name(std::move(name)), code(std::move(code)), patches(std::move(patches)) {}

    void patch(
        uint8_t* function_pointer,
        std::size_t& function_offset,
        uint8_t* got_pointer,
        std::size_t& got_offset,
        std::initializer_list<CnpPatchValue> values
    ) const;

    CnpStencil() = default;
} ;
