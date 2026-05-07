#pragma once
#include <cstdint>
#include <string>


enum class Opcode : uint8_t {
    #define DEF_INSTR(opcode, name, size, stencil, trim) opcode,
    #include "Instructions.inc"
    #undef DEF_INSTR
    NUM_OPCODES
};

class OpcodeUtils {
public:
    static std::string to_string(Opcode opcode);
    static std::size_t size(Opcode opcode);
    static std::size_t opcode_count();
};


