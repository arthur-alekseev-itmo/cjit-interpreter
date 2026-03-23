#include "ElfBinaryWrapper.h"

ElfBinaryWrapper::ElfBinaryWrapper(const std::string& path) {

}

const SymbolWrapper* ElfBinaryWrapper::get_symbol(const std::string& name) const {

}

const LIEF::Section* ElfBinaryWrapper::get_text_section() const {

}

const std::vector<RelocationWrapper> ElfBinaryWrapper::get_relocations_for_symbol(const SymbolWrapper* symbol) const {

}
