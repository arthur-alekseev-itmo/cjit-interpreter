#include "ir.hpp"

#include <cassert>

std::size_t opcode_size(const opcode oc) {
    switch (oc) {
    case NOP:
    case ADD:
    case SUB:
    case EXIT:
    case DUP:
    case DROP:
    case RETURN:
    case EQ:
    case MUL: return 1;
    case CALL:
    case JUMP:
    case JUMP_TRUE:
    case READ_STACK:
    case WRITE_STACK: return 5;
    case LOAD_IMM:
    case CALL_C_V_STACK_PTR:
    case CALL_C_V_U64: return 9;
    default: assert(false);
    }
}

void my_print(uint64_t argument) {
     std::cout << argument << std::endl;
}

void my_debug_view(uint64_t* stack) {

}
