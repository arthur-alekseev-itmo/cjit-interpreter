#include "MachOBinaryWrapper.hpp"

#include <iostream>

MachOBinaryWrapper::MachOBinaryWrapper(const std::string& path) {
    const auto config = LIEF::MachO::ParserConfig::quick();
    const auto fat = LIEF::MachO::Parser::parse(path, config);

    if (!fat || fat->empty()) {
        throw std::runtime_error("Unable to read mach-o file at " + path);
    }

    auto bin = fat->take(0);

    if (bin->header().file_type() != LIEF::MachO::FILE_TYPES::MH_OBJECT) {
        throw std::runtime_error("Unable to locate mach-o fit binary in parsed fat binary");
    }

    binary_ = std::move(bin);
}

const LIEF::Symbol* MachOBinaryWrapper::get_symbol(const std::string& name) const {
    return binary_->get_symbol("_" + name);
}

const LIEF::Section* MachOBinaryWrapper::get_text_section() const {
    for (const auto& section : binary_->symbols()){
        std::cout << section.name() << std::endl;
    }
    return binary_->get_section(text_section_name_);
}

const std::vector<LIEF::Relocation> MachOBinaryWrapper::get_relocations_for_symbol(const LIEF::Symbol* symbol) const {
    auto result = std::vector<LIEF::Relocation>();
    const auto stencil_size = symbol->size();

    for (const LIEF::MachO::Relocation& relocation : binary_->relocations()) {
        const auto address = relocation.address();
        if (!relocation.has_section() || relocation.section()->name() != text_section_name_) {
            continue;
        }
        if (address < symbol->value() || address >= symbol->value() + stencil_size) {
            continue;
        }
        result.push_back(relocation);
    }

    return result;
}
