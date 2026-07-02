#pragma once
#include <cstddef>
#include <initializer_list>

#include "../Parsing/Stencil/Data/CnpStencil.h"

struct CnpPatchValue {
    std::size_t size;
    const std::uint8_t* address;

    CnpPatchValue(
        const std::size_t size, const std::uint8_t* address
    ) : size(size), address(address) {}
};

class CnpPatcher {
public:
    static void patch(
        const CnpStencil& stencil,
        uint8_t* function_pointer,
        std::size_t& function_offset,
        uint8_t* got_pointer,
        std::size_t& got_offset,
        std::initializer_list<CnpPatchValue> values
    );
};
