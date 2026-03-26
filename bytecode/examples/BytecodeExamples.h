#pragma once
#include <cstdint>
#include "../Opcode.h"
#include "../Bytecode.h"

#define OP(op) static_cast<uint8_t>(Opcode::op)

inline std::uint8_t basic_math_example[] = {
    OP(LOAD_IMM), CONST_64(0x03),      // Load 3
    OP(LOAD_IMM), CONST_64(0x05),      // Load 5
    OP(MUL),                           // Mul
    OP(LOAD_IMM), CONST_64(0x07),      // Load 7
    OP(ADD),                           // Add
    OP(RETURN)                         // Return
};

inline uint8_t infinite_loop_example[] = {
    OP(LOAD_IMM), CONST_64(0x00),      // Load 0
    OP(LOAD_IMM), CONST_64(0x01),      // Load 1
    OP(ADD),                           // Add
    OP(DUP),                           // Dup
    OP(CALL_BUILTIN), CONST_32(0),     // Print
    OP(JUMP), CONST_32(0x01)           // Jump 0x01
};

inline uint8_t print_42[] = {
    OP(LOAD_IMM), CONST_64(42),        // Load 42
    OP(CALL_BUILTIN), CONST_32(0),     // Print
    OP(RETURN)                         // Return
};

inline uint8_t print_10_factorial[] = {
    /* main */
    /* 00 */ OP(LOAD_IMM), CONST_64(0xa),
    /* 01 */ OP(CALL), CONST_32(0x4),
    /* 02 */ OP(CALL_BUILTIN), CONST_32(0),
    /* 03 */ OP(RETURN),

    /* factorial */
    /* 04 */ OP(DUP),
    /* 05 */ OP(LOAD_IMM), CONST_64(0x1),
    /* 06 */ OP(EQ),
    /* 07 */ OP(JUMP_TRUE), CONST_32(0xd),
    /* 08 */ OP(DUP),
    /* 09 */ OP(LOAD_IMM), CONST_64(0x1),
    /* 0a */ OP(SUB),
    /* 0b */ OP(CALL), CONST_32(0x4),
    /* 0c */ OP(MUL),
    /* 0d */ OP(RETURN)
};

inline uint8_t print_10_fib[] = {
    /* main */
    /* 00 */ OP(LOAD_IMM), CONST_64(10),
    /* 01 */ OP(CALL), CONST_32(0x4),

    /* 02 */ OP(CALL_BUILTIN), CONST_32(0),
    /* 03 */ OP(RETURN),


    /* fib */
    /* 04 */ OP(DUP),
    /* 05 */ OP(LOAD_IMM), CONST_64(0x1),
    /* 06 */ OP(EQ),
    /* 07 */ OP(JUMP_TRUE), CONST_32(0x16),

    /* 08 */ OP(DUP),
    /* 09 */ OP(LOAD_IMM), CONST_64(0x2),
    /* 0a */ OP(EQ),
    /* 0b */ OP(JUMP_TRUE), CONST_32(0x16),

    /* 0c */ OP(DUP),
    /* 0d */ OP(LOAD_IMM), CONST_64(0x1),
    /* 0e */ OP(SUB),
    /* 0f */ OP(CALL), CONST_32(0x4),

    /* 10 */ OP(SWAP),
    /* 11 */ OP(LOAD_IMM), CONST_64(0x2),
    /* 12 */ OP(SUB),
    /* 13 */ OP(CALL), CONST_32(0x4),

    /* 14 */ OP(ADD),
    /* 15 */ OP(RETURN),

    /* 16 */ OP(DROP),
    /* 17 */ OP(LOAD_IMM), CONST_64(0x1),
    /* 18 */ OP(RETURN),
};
