#include "ir.hpp"

std::size_t opcode_size(opcode oc) {
    switch (oc) {
    case NOP:
    case ADD:
    case EXIT:
    case DUP:
    case DROP:
    case RETURN:
    case MUL: return 1;
    case JUMP:
    case LOAD_IMM: return 5;
    case CALL_C_V_U64: return 9;
    default: assert(false);
    }
}
