#pragma once
#include <cstdint>
#include <string>


enum class Opcode : uint8_t {
    NOP = 0x0,
    LOAD_IMM = 0x1,
    ADD = 0x2,
    SUB = 0x3,
    MUL = 0x4,
    EQ = 0x5,
    CALL_C_V_U64 = 0x6,
    CALL_C_V_STACK_PTR = 0x7,
    EXIT = 0x8,
    DUP = 0x9,
    DROP = 0xA,
    JUMP = 0xB,
    JUMP_TRUE = 0xC,
    RETURN = 0xD,
    READ_STACK = 0xE,
    WRITE_STACK = 0xF,
    CALL = 0x10,
    SWAP = 0x11,
    CALL_BUILTIN = 0x12,

    NUM_OPCODES
};

class OpcodeUtils {
public:
    static std::string to_string(Opcode opcode);
    static std::size_t size(Opcode opcode);
    static std::size_t opcode_count();
};


