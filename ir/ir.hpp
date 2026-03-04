#pragma once
#include <cassert>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

typedef const std::vector<uint8_t> bytecode;
typedef const uint8_t* bytecode_ptr;

enum opcode {
    NOP = 0x0,
    LOAD_IMM = 0x1,
    ADD = 0x2,
    MUL = 0x3,
    CALL_C_V_U64 = 0x4,
    EXIT = 0x5,
    DUP = 0x6,
    DROP = 0x7,
    JUMP = 0x8,
    RETURN = 0x9,

    NUM_OPCODES
};

// TODO: Unify with declaration
std::size_t opcode_size(opcode oc);

inline void my_print(std::uint64_t value) {
    std::cout << value << std::endl;
}

inline const uint8_t* print_addr = reinterpret_cast<uint8_t*>(&my_print);
static_assert(sizeof(&my_print) == 8);


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
    CALL_C_V_U64,                     // Print
    JUMP, 0x01, 0x00, 0x00, 0x00      // Jump 0x01
};

inline uint8_t print_42[] = {
    LOAD_IMM, 0x2a, 0x00, 0x00, 0x00, // Load 42
    CALL_C_V_U64,                     // Print
    RETURN                            // Return
};
