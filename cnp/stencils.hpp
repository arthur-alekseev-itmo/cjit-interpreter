#pragma once
#include <memory>
#include <vector>

struct stencil {
    std::unique_ptr<std::vector<uint8_t>> code;
    std::unique_ptr<std::vector<std::size_t>> patches_addresses;

    stencil(
        std::unique_ptr<std::vector<uint8_t>> code,
        std::unique_ptr<std::vector<std::size_t>> patches_addresses
    ) : code(std::move(code)), patches_addresses(std::move(patches_addresses)) {}

    // TODO: Remove bad state
    stencil() : code(nullptr), patches_addresses(nullptr) {}

    stencil& operator=(const stencil&) = delete;

    stencil& operator=(stencil&& other) noexcept = default;
} ;

// Call compiler for stencils project
// Parse object file and create stencil structs
std::unique_ptr<std::vector<stencil>> create_stencils();
