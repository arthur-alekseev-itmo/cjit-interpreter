#pragma once
#include "BinaryWrapper.hpp"


class ElfBinaryWrapper final : public BinaryWrapper {
public:
    explicit ElfBinaryWrapper(const std::string& path);

    [[nodiscard]] const SymbolWrapper* get_symbol(const std::string& name) const override;
    [[nodiscard]] const LIEF::Section* get_text_section() const override;
    std::vector<RelocationWrapper> get_relocations_for_symbol(const SymbolWrapper* symbol, bool cut_jump) const override;

private:
    std::unique_ptr<const LIEF::ELF::Binary> binary_;
    std::unordered_map<std::string, SymbolWrapper> symbols_;
    const std::string text_section_name_ = ".text";

    void fill_symbols();
};