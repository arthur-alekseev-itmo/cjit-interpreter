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

    CnpStencil() = default;
} ;
