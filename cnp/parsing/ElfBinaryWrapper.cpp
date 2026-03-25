#include "ElfBinaryWrapper.h"

ElfBinaryWrapper::ElfBinaryWrapper(const std::string& path) {

}

SymbolWrapper* ElfBinaryWrapper::get_symbol(const std::string& name) const {

}

LIEF::Section* ElfBinaryWrapper::get_text_section() const {

}

std::vector<RelocationWrapper> ElfBinaryWrapper::get_relocations_for_symbol(const SymbolWrapper* symbol, bool cut_jump) const {

}
