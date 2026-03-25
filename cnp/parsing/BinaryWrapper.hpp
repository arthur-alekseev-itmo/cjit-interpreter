#pragma once
#include <LIEF/LIEF.hpp>

#include "RelocationWrapper.h"
#include "SymbolWrapper.h"

class BinaryWrapper {
public:
    virtual ~BinaryWrapper() = default;
    [[nodiscard]] virtual const SymbolWrapper* get_symbol(const std::string& name) const = 0;
    [[nodiscard]] virtual const LIEF::Section* get_text_section() const = 0;
    virtual std::vector<RelocationWrapper> get_relocations_for_symbol(const SymbolWrapper* symbol, bool cut_jump) const = 0;
};


