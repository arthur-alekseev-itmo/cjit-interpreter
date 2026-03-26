#include "Opcode.h"
#include <stdexcept>

std::string OpcodeUtils::to_string(const Opcode opcode) {
    switch (opcode) {
    case Opcode::NOP: return "NOP";
    case Opcode::LOAD_IMM: return "LOAD_IMM";
    case Opcode::ADD: return "ADD";
    case Opcode::SUB: return "SUB";
    case Opcode::MUL: return "MUL";
    case Opcode::EQ: return "EQ";
    case Opcode::CALL_C_V_U64: return "CALL_C_V_U64";
    case Opcode::CALL_C_V_STACK_PTR: return "CALL_C_V_STACK_PTR";
    case Opcode::EXIT: return "EXIT";
    case Opcode::DUP: return "DUP";
    case Opcode::DROP: return "DROP";
    case Opcode::JUMP: return "JUMP";
    case Opcode::JUMP_TRUE: return "JUMP_TRUE";
    case Opcode::RETURN: return "RETURN";
    case Opcode::READ_STACK: return "READ_STACK";
    case Opcode::WRITE_STACK: return "WRITE_STACK";
    case Opcode::CALL: return "CALL";
    case Opcode::SWAP: return "SWAP";
    case Opcode::CALL_BUILTIN: return "CALL_BUILTIN";
    default: throw std::runtime_error("Unknown opcode");
    }
}

std::size_t OpcodeUtils::size(const Opcode opcode) {
    switch (opcode) {
    case Opcode::NOP: return 1;
    case Opcode::LOAD_IMM: return 9;
    case Opcode::ADD: return 1;
    case Opcode::SUB: return 1;
    case Opcode::MUL: return 1;
    case Opcode::EQ: return 1;
    case Opcode::CALL_C_V_U64: return 9;
    case Opcode::CALL_C_V_STACK_PTR: return 9;
    case Opcode::EXIT: return 1;
    case Opcode::DUP: return 1;
    case Opcode::DROP: return 1;
    case Opcode::JUMP: return 5;
    case Opcode::JUMP_TRUE: return 5;
    case Opcode::RETURN: return 1;
    case Opcode::READ_STACK: return 5;
    case Opcode::WRITE_STACK: return 5;
    case Opcode::CALL: return 5;
    case Opcode::SWAP: return 1;
    case Opcode::CALL_BUILTIN: return 5;
    default: throw std::runtime_error("Unknown opcode");
    }
}

std::size_t OpcodeUtils::opcode_count() {
    return static_cast<std::size_t>(Opcode::NUM_OPCODES);
}
