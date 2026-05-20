#pragma once
#include <cstddef>
#include <cstdint>

class RelocationWrapper {
public:
    std::size_t address;
    uint32_t type;
    std::uint8_t architecture;

    RelocationWrapper(
        const std::size_t address,
        const uint32_t type,
        const std::uint8_t architecture
    ) : address(address), type(type), architecture(architecture) {}
};
