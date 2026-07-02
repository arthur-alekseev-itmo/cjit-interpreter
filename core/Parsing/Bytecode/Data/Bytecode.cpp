#include "Bytecode.h"

#include <cstring>

Bytecode::Bytecode(const uint8_t* source, std::size_t size) {
    content_ = std::vector<uint8_t>(size);
    memcpy(content_.data(), source, size);
}

std::size_t Bytecode::size() const {
    return content_.size();
}

const uint8_t* Bytecode::data() const {
    return content_.data();
}
