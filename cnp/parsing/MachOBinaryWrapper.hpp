#pragma once
#include "BinaryWrapper.hpp"
#include <LIEF/LIEF.hpp>

class MachOBinaryWrapper final : public BinaryWrapper {
public:
    explicit MachOBinaryWrapper(const std::string& path);

    [[nodiscard]] const SymbolWrapper* get_symbol(const std::string& name) const override;
    [[nodiscard]] const LIEF::Section* get_text_section() const override;
    const std::vector<LIEF::Relocation> get_relocations_for_symbol(const SymbolWrapper* symbol) const override;

private:
    std::unordered_map<std::string, SymbolWrapper> symbols_;
    std::unique_ptr<const LIEF::MachO::Binary> binary_;
    const std::string text_section_name_ = "__text";

    void fill_symbols();
};
