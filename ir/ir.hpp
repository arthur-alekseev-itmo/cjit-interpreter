#pragma once

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

    NUM_OPCODES
};

// TODO: Unify with declaration
std::size_t opcode_size(opcode oc);

void my_print(uint64_t argument);
void my_debug_view(uint64_t* stack);

inline auto my_print_addr = reinterpret_cast<uintptr_t>(&my_print);
inline auto my_debug_view_addr = reinterpret_cast<uintptr_t>(&my_debug_view);

static_assert(sizeof(&my_print) == 8);

// Takes a byte from a number at a given offset
#define BYTE_IDX(name, idx) *(reinterpret_cast<uint8_t*>(&(name)) + (idx))

// Converts 64-bit number to comma-separated bytes
// Example: 0xAABBCCDD00112233 -> 0x33, 0x22, 0x11, 0x00, 0xDD, 0xCC, 0xBB, 0xAA
#define U64_TO_BYTES(name) \
  BYTE_IDX(name, 0), BYTE_IDX(name, 1), BYTE_IDX(name, 2), BYTE_IDX(name, 3), \
  BYTE_IDX(name, 4), BYTE_IDX(name, 5), BYTE_IDX(name, 6), BYTE_IDX(name, 7)

#define U32_TO_BYTES(name) \
  BYTE_IDX(name, 0), BYTE_IDX(name, 1), BYTE_IDX(name, 2), BYTE_IDX(name, 3)

#define MY_PRINT_BYTES U64_TO_BYTES(my_print_addr)
#define MY_DEBUG_VIEW_BYTES U64_TO_BYTES(my_debug_view_addr)

#define CONST_64(value) (uint8_t)value, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define CONST_32(value) (uint8_t)value, 0x00, 0x00, 0x00

inline uint8_t basic_math_example[] = {
    LOAD_IMM, CONST_64(0x03),      // Load 3
    LOAD_IMM, CONST_64(0x05),      // Load 5
    MUL,                              // Mul
    LOAD_IMM, CONST_64(0x07),      // Load 7
    ADD,                              // Add
    RETURN                            // Return
};

inline uint8_t infinite_loop_example[] = {
    LOAD_IMM, CONST_64(0x00),      // Load 0
    LOAD_IMM, CONST_64(0x01),      // Load 1
    ADD,                              // Add
    DUP,                              // Dup
    CALL_C_V_U64, MY_PRINT_BYTES,     // Print
    JUMP, CONST_32(0x01)           // Jump 0x01
};

inline uint8_t print_42[] = {
    LOAD_IMM, CONST_64(42),        // Load 42
    CALL_C_V_U64, MY_PRINT_BYTES,     // Print
    RETURN                            // Return
};

inline uint8_t print_10_factorial[] = {
    /* main */
    /* 00 */ LOAD_IMM, CONST_64(0xa),
    /* 01 */ CALL, CONST_32(0x4),
    /* 02 */ CALL_C_V_U64, MY_PRINT_BYTES,
    /* 03 */ RETURN,

    /* factorial */
    /* 04 */ DUP,
    /* 05 */ LOAD_IMM, CONST_64(0x1),
    /* 06 */ EQ,
    /* 07 */ JUMP_TRUE, CONST_32(0xd),
    /* 08 */ DUP,
    /* 09 */ LOAD_IMM, CONST_64(0x1),
    /* 0a */ SUB,
    /* 0b */ CALL, CONST_32(0x4),
    /* 0c */ MUL,
    /* 0d */ RETURN
};
