#pragma once
#include <cstddef>
#include <cstdint>

#include "../../cmake-build-debug/_deps/lief-src/include/LIEF/Abstract/enums.hpp"


class RelocationWrapper {
public:
    std::size_t address;
    std::uint32_t type;
    std::uint8_t architecture;
};
