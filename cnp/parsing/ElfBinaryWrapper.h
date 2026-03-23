#pragma once
#include "BinaryWrapper.hpp"


class ElfBinaryWrapper : public BinaryWrapper {
public:
    explicit ElfBinaryWrapper(const std::string& path);

    [[nodiscard]] const SymbolWrapper* get_symbol(const std::string& name) const override;
    [[nodiscard]] const LIEF::Section* get_text_section() const override;
    const std::vector<RelocationWrapper> get_relocations_for_symbol(const SymbolWrapper* symbol) const override;

};