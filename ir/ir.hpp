#pragma once
#include <cstdint>
#include <vector>

typedef const std::vector<uint8_t> bytecode;
typedef const uint8_t* bytecode_ptr;

enum opcode {
    NOP = 0x0,
    LOAD_IMM = 0x1,
    ADD = 0x2,
    MUL = 0x3,
    PRINT = 0x4,
    EXIT = 0x5,
    DUP = 0x6,
    JUMP = 0x7,
    RETURN = 0x8
};

inline uint8_t basic_math_example[] = {
    LOAD_IMM, 0x03, 0x00, 0x00, 0x00, // Load 3
    LOAD_IMM, 0x05, 0x00, 0x00, 0x00, // Load 5
    MUL,                              // Mul
    LOAD_IMM, 0x07, 0x00, 0x00, 0x00, // Load 7
    ADD,                              // Add
    RETURN                            // Return
};

inline uint8_t infinite_loop_example[] = {
    LOAD_IMM, 0x00, 0x00, 0x00, 0x00, // Load 0
    LOAD_IMM, 0x01, 0x00, 0x00, 0x00, // Load 1
    ADD,                              // Add
    DUP,                              // Dup
    PRINT,                            // Print
    JUMP, 0x01, 0x00, 0x00, 0x00      // Jump 0x01
};
