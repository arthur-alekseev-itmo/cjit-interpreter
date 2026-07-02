#pragma once

#include <memory>

#include "LIEF/LIEF.hpp"

class SymbolWrapper {
public:
    SymbolWrapper(const LIEF::Symbol& symbol, std::size_t size);

    [[nodiscard]] std::string name() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] std::size_t offset() const;
    [[nodiscard]] LIEF::Symbol* symbol() const;

private:
    const std::size_t offset_;
    const std::size_t size_;
    std::unique_ptr<LIEF::Symbol> symbol_;
};