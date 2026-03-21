#pragma once

#include <algorithm>
#include <utility>
#include <vector>
#include <memory>

#include "../../cmake-build-debug/_deps/lief-src/include/LIEF/ELF/enums.hpp"
#include "LIEF/MachO/enums.hpp"

struct CnpStencilPatch {
    LIEF::ELF::RELOC_x86_64 type; // TODO: It is stored as uint8_t in Relocation.cpp, maybe copy it
    std::size_t address;

    CnpStencilPatch(
        const LIEF::ELF::RELOC_x86_64 type,
        const std::size_t address
    ) : type(type), address(address) {}
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
