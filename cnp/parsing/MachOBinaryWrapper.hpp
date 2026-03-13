#pragma once
#include "BinaryWrapper.hpp"
#include <LIEF/LIEF.hpp>

class MachOBinaryWrapper final : public BinaryWrapper {
public:
    explicit MachOBinaryWrapper(const std::string& path);

    const LIEF::Symbol* get_symbol(const std::string& name) const override;
    const LIEF::Section* get_text_section() const override;
    const std::vector<LIEF::Relocation> get_relocations_for_symbol(const LIEF::Symbol* symbol) const override;

private:
    std::unique_ptr<const LIEF::MachO::Binary> binary_;
    const std::string text_section_name_ = "__TEXT";
};
