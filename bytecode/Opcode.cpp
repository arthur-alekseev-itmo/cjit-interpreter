#include "Opcode.h"
#include <stdexcept>

std::string OpcodeUtils::to_string(const Opcode opcode) {
    switch (opcode) {
    case Opcode::NOP: return "NOP";
    case Opcode::LOAD_IMM: return "LOAD_IMM";
    case Opcode::ADD: return "ADD";
    case Opcode::SUB: return "SUB";
    case Opcode::MUL: return "MUL";
    case Opcode::DIV: return "DIV";
    case Opcode::EQ: return "EQ";
    case Opcode::NEQ: return "NEQ";
    case Opcode::LE: return "LE";
    case Opcode::GE: return "GE";
    case Opcode::LT: return "LT";
    case Opcode::GT: return "GT";
    case Opcode::CALL_C_V_U64: return "CALL_C_V_U64";
    case Opcode::CALL_C_V_STACK_PTR: return "CALL_C_V_STACK_PTR";
    case Opcode::EXIT: return "EXIT";
    case Opcode::DUP: return "DUP";
    case Opcode::DROP: return "DROP";
    case Opcode::JUMP: return "JUMP";
    case Opcode::JUMP_TRUE: return "JUMP_TRUE";
    case Opcode::JUMP_FALSE: return "JUMP_FALSE";
    case Opcode::RETURN: return "RETURN";
    case Opcode::READ_STACK: return "READ_STACK";
    case Opcode::WRITE_STACK: return "WRITE_STACK";
    case Opcode::LOAD_LOCAL: return "LOAD_LOCAL";
    case Opcode::STORE_LOCAL: return "STORE_LOCAL";
    case Opcode::GET_LOCAL_REF: return "GET_LOCAL_REF";
    case Opcode::ALLOCATE_LOCALS: return "ALLOCATE_LOCALS";
    case Opcode::CALL: return "CALL";
    case Opcode::CALL_OBJECT: return "CALL_OBJECT";
    case Opcode::CALL_BUILTIN: return "CALL_BUILTIN";
    case Opcode::SWAP: return "SWAP";
    case Opcode::NEG: return "NEG";
    case Opcode::NOT: return "NOT";
    case Opcode::WRITE_TO_REF: return "WRITE_TO_REF";
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
    case Opcode::DIV: return 1;
    case Opcode::EQ: return 1;
    case Opcode::NEQ: return 1;
    case Opcode::LE: return 1;
    case Opcode::GE: return 1;
    case Opcode::LT: return 1;
    case Opcode::GT: return 1;
    case Opcode::CALL_C_V_U64: return 9;
    case Opcode::CALL_C_V_STACK_PTR: return 9;
    case Opcode::EXIT: return 1;
    case Opcode::DUP: return 1;
    case Opcode::DROP: return 1;
    case Opcode::JUMP: return 5;
    case Opcode::JUMP_TRUE: return 5;
    case Opcode::JUMP_FALSE: return 5;
    case Opcode::RETURN: return 1;
    case Opcode::READ_STACK: return 5;
    case Opcode::WRITE_STACK: return 5;
    case Opcode::LOAD_LOCAL: return 5;
    case Opcode::STORE_LOCAL: return 5;
    case Opcode::GET_LOCAL_REF: return 5;
    case Opcode::ALLOCATE_LOCALS: return 5;
    case Opcode::CALL: return 5;
    case Opcode::CALL_OBJECT: return 1;
    case Opcode::CALL_BUILTIN: return 5;
    case Opcode::SWAP: return 1;
    case Opcode::NEG: return 1;
    case Opcode::NOT: return 1;
    case Opcode::WRITE_TO_REF: return 1;
    default: throw std::runtime_error("Unknown opcode");
    }
}

std::size_t OpcodeUtils::opcode_count() {
    return static_cast<std::size_t>(Opcode::NUM_OPCODES);
}