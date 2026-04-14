#include "ElfBinaryWrapper.h"

ElfBinaryWrapper::ElfBinaryWrapper(const std::string& path) {
    binary_ = LIEF::ELF::Parser::parse(path);
    symbols_ = std::unordered_map<std::string, SymbolWrapper>();
    fill_symbols();
}

void ElfBinaryWrapper::fill_symbols() {
    for (const auto& symbol : binary_->symbols()) {
        symbols_.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(symbol.name()),
            std::forward_as_tuple(symbol, symbol.size())
        );
    }
}

const SymbolWrapper* ElfBinaryWrapper::get_symbol(const std::string& name) const {
    if (!symbols_.contains(name)) {
        throw std::runtime_error("Cannot find stencil with name: " + name);
    }
    return std::addressof(symbols_.at(name));
}

const LIEF::Section* ElfBinaryWrapper::get_text_section() const {
    const auto section = binary_->get_section(text_section_name_);
    return section;
}

#define JUMP_SIZE 5

std::vector<RelocationWrapper> ElfBinaryWrapper::get_relocations_for_symbol(const SymbolWrapper* symbol, const bool cut_jump) const {
    const auto relocations = binary_->relocations();
    auto result = std::vector<RelocationWrapper>();
    const auto stencil_size = symbol->size();
    const auto jump_size = cut_jump ? JUMP_SIZE : 0;

    for (const LIEF::ELF::Relocation& relocation : relocations) {
        const auto address = relocation.address();

        if (!relocation.has_section() || relocation.section()->name() != ".text") {
            continue;
        }

        if (address < symbol->offset() || address >= symbol->offset() + stencil_size - jump_size) {
            continue;
        }

        result.emplace_back(
            address,
            relocation.type(),
            static_cast<uint8_t>(relocation.architecture())
        );
    }

    return result;
}
