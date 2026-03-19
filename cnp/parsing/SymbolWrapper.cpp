#include "SymbolWrapper.h"

SymbolWrapper::SymbolWrapper(
    const LIEF::Symbol& symbol, std::size_t size
) : offset_(symbol.value()), size_(size), symbol_(std::make_unique<LIEF::Symbol>(symbol)) { }

std::string SymbolWrapper::name() const {
    return symbol_->name();
}

std::size_t SymbolWrapper::size() const {
    return size_;
}

std::size_t SymbolWrapper::offset() const {
    return offset_;
}

LIEF::Symbol* SymbolWrapper::symbol() const {
    return symbol_.get();
}




