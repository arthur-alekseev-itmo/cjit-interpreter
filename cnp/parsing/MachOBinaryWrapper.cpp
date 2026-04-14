#include "MachOBinaryWrapper.hpp"

#include <iostream>
#include <algorithm>
#include <ranges>

#include "SymbolWrapper.h"

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
    symbols_ = std::unordered_map<std::string, SymbolWrapper>();
    fill_symbols();
}

void MachOBinaryWrapper::fill_symbols() {
    const auto text_section = get_text_section();
    auto sorted_symbols = std::vector<LIEF::MachO::Symbol>();
    for (const LIEF::MachO::Symbol& symbol : binary_->symbols()) {
        if (symbol.category() != LIEF::MachO::Symbol::CATEGORY::EXTERNAL){
            continue;
        }
        sorted_symbols.push_back(symbol);
    }

    std::ranges::sort(
        sorted_symbols,
        [](const LIEF::MachO::Symbol& a, const LIEF::MachO::Symbol& b) { return a.value() < b.value(); }
    );

    for (auto i = 0; i < sorted_symbols.size() - 1; i++) {
        const LIEF::MachO::Symbol& current = sorted_symbols[i];
        const LIEF::MachO::Symbol& next = sorted_symbols[i + 1];

        symbols_.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(current.name()),
            std::forward_as_tuple(current, next.value() - current.value())
        );
    }

    const auto last = sorted_symbols[sorted_symbols.size() - 1];
    symbols_.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(last.name()),
        std::forward_as_tuple(last, text_section->content().size() - last.value())
    );
}

const SymbolWrapper* MachOBinaryWrapper::get_symbol(const std::string& name) const {
    return std::addressof(symbols_.at("_" + name));
}

const LIEF::Section* MachOBinaryWrapper::get_text_section() const {
    return binary_->get_section(text_section_name_);
}

#define JUMP_SIZE 4

std::vector<RelocationWrapper> MachOBinaryWrapper::get_relocations_for_symbol(
    const SymbolWrapper* symbol,
    bool cut_jump
) const {
    auto result = std::vector<RelocationWrapper>();
    const auto stencil_size = symbol->size();
    const auto jump_size = cut_jump ? JUMP_SIZE : 0;
    for (const LIEF::MachO::Relocation& relocation : binary_->relocations()) {
        const auto address = relocation.address() - relocation.section()->offset();

        if (!relocation.has_section() || relocation.section()->name() != text_section_name_) {
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