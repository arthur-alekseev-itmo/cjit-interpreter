
#include "CnpExecutionTrace.h"

// I don't want to deal with C++ goofy method pointers etc
uint64_t* global_base_;
std::size_t global_current_instruction_idx_ = 0;

CnpExecutionTrace::CnpExecutionTrace(uint64_t* base) : base_(base) {
    global_base_ = base;
}

void trace(const uint64_t* stack) {
    const uint64_t* stack_ptr = stack;
    std::cout << global_current_instruction_idx_++ << "[";
    while (stack_ptr > global_base_) {
        std::cout << " " << *(--stack_ptr);
    }
    std::cout << " ]" << std::endl;
}

bytecode CnpExecutionTrace::instrument(bytecode& bc) {
    auto result = std::vector<uint8_t>();
    auto trace_bytes = reinterpret_cast<uintptr_t>(&trace);
    const uint8_t debug_instruction[] = { CALL_C_V_STACK_PTR, U64_TO_BYTES(trace_bytes) };
    std::size_t ip = 0;

    while (ip < bc.size()) {
        const auto op = static_cast<opcode>(bc[ip]);
        const auto step = opcode_size(op);
        switch (op) {
        case JUMP:
        case JUMP_TRUE:
        case CALL:
            {
                const auto fst_arg = *reinterpret_cast<const uint32_t*>(bc.data() + ip + 1);
                const auto doubled = fst_arg * 2;
                result.push_back(bc[ip]);
                for (std::size_t j = 0; j < sizeof(uint32_t); j++) {
                    result.push_back(*(reinterpret_cast<const uint8_t*>(&doubled) + j));
                }
                break;
            }
        default:
            for (std::size_t j = 0; j < step; j++) {
                result.push_back(bc[ip + j]);
            }
        }

        for (unsigned char trace_byte : debug_instruction) {
            result.push_back(trace_byte);
        }
        ip += step;
    }

    return result;
}
