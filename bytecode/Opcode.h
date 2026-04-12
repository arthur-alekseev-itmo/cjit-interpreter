#pragma once
#include <cstdint>
#include <string>


enum class Opcode : uint8_t {
    NOP = 0x0,
    LOAD_IMM = 0x1,
    ADD = 0x2,
    SUB = 0x3,
    MUL = 0x4,
    DIV = 0x5,
    EQ = 0x6,
    NEQ = 0x7,
    LE = 0x8,
    GE = 0x9,
    LT = 0xA,
    GT = 0xB,
    CALL_C_V_U64 = 0xC,
    CALL_C_V_STACK_PTR = 0xD,
    EXIT = 0xE,
    DUP = 0xF,
    DROP = 0x10,
    JUMP = 0x11,
    JUMP_TRUE = 0x12,
    JUMP_FALSE = 0x13,
    RETURN = 0x14,
    READ_STACK = 0x15,
    WRITE_STACK = 0x16,
    LOAD_LOCAL = 0x17,
    STORE_LOCAL = 0x18,
    GET_LOCAL_REF = 0x19,
    ALLOCATE_LOCALS = 0x1A,
    CALL = 0x1B,
    CALL_OBJECT = 0x1C,
    CALL_BUILTIN = 0x1D,
    SWAP = 0x1E,
    NEG = 0x1F,
    NOT = 0x20,
    WRITE_TO_REF = 0x21,

    NUM_OPCODES
};

class OpcodeUtils {
public:
    static std::string to_string(Opcode opcode);
    static std::size_t size(Opcode opcode);
    static std::size_t opcode_count();
};


