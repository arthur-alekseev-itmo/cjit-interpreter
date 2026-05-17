#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

// Takes a byte from a number at a given offset
#define BYTE_IDX(name, idx) *(reinterpret_cast<uint8_t*>(&(name)) + (idx))

// Converts 64-bit number to comma-separated bytes
// Example: 0xAABBCCDD00112233 -> 0x33, 0x22, 0x11, 0x00, 0xDD, 0xCC, 0xBB, 0xAA
#define U64_TO_BYTES(name) \
BYTE_IDX(name, 0), BYTE_IDX(name, 1), BYTE_IDX(name, 2), BYTE_IDX(name, 3), \
BYTE_IDX(name, 4), BYTE_IDX(name, 5), BYTE_IDX(name, 6), BYTE_IDX(name, 7)

#define U32_TO_BYTES(name) \
BYTE_IDX(name, 0), BYTE_IDX(name, 1), BYTE_IDX(name, 2), BYTE_IDX(name, 3)

#define MY_PRINT_BYTES U64_TO_BYTES(my_print_addr)
#define MY_DEBUG_VIEW_BYTES U64_TO_BYTES(my_debug_view_addr)

#define CONST_64(value) (uint8_t)value, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define CONST_32(value) (uint8_t)value, 0x00, 0x00, 0x00

class Bytecode {
public:
    Bytecode(const uint8_t* source, std::size_t size);
    explicit Bytecode(std::vector<uint8_t>&& content) : content_(std::move(content)) {}

    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] const uint8_t* data() const;

private:
    std::vector<uint8_t> content_;
};
