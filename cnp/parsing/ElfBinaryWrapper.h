#pragma once
#include "BinaryWrapper.hpp"


class ElfBinaryWrapper : public BinaryWrapper {
public:
    explicit ElfBinaryWrapper(const std::string& path);

    [[nodiscard]] SymbolWrapper* get_symbol(const std::string& name) const override;
    [[nodiscard]] LIEF::Section* get_text_section() const override;
    std::vector<RelocationWrapper> get_relocations_for_symbol(const SymbolWrapper* symbol, bool cut_jump) const override;

};