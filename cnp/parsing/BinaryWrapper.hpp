#pragma once
#include <LIEF/LIEF.hpp>

#include "SymbolWrapper.h"

class BinaryWrapper {
public:
    virtual ~BinaryWrapper() = default;
    virtual const SymbolWrapper* get_symbol(const std::string& name) const = 0;
    virtual const LIEF::Section* get_text_section() const = 0;
    virtual const std::vector<LIEF::Relocation> get_relocations_for_symbol(const SymbolWrapper* symbol) const = 0;
};


