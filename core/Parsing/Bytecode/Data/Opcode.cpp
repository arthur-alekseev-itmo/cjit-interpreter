#include "Opcode.h"

#include <stdexcept>

std::string OpcodeUtils::to_string(const Opcode opcode) {
    switch (opcode) {
    #define DEF_INSTR(opcode, name, size, stencil, trim) case Opcode::opcode: return name;
    #include "Instructions.inc"
    #undef DEF_INSTR
    default: throw std::runtime_error("Unknown opcode");
    }
}

std::size_t OpcodeUtils::size(const Opcode opcode) {
    switch (opcode) {
    #define DEF_INSTR(opcode, name, size, stencil, trim) case Opcode::opcode: return size;
    #include "Instructions.inc"
    #undef DEF_INSTR
    default: throw std::runtime_error("Unknown opcode");
    }
}

std::size_t OpcodeUtils::opcode_count() {
    return static_cast<std::size_t>(Opcode::NUM_OPCODES);
}