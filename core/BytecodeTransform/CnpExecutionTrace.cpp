
#include "CnpExecutionTrace.h"

#include <cstddef>
#include <iostream>

#include "../Parsing/Bytecode/Data/Opcode.h"
#include "../Parsing/Bytecode/Data/Bytecode.h"

enum class Opcode : uint8_t;
// I don't want to deal with C++ goofy method pointers etc
uint64_t* global_base_;
uint64_t* global_locals_;
std::size_t global_current_instruction_idx_ = 0;

CnpExecutionTrace::CnpExecutionTrace(uint64_t* stack_base, uint64_t* locals_base) : base_(stack_base) {
    global_base_ = stack_base;
    global_locals_ = locals_base;
}

void trace(const uint64_t* stack, const uint64_t* locals) {
    const uint64_t* stack_ptr = stack;
    if (stack_ptr - global_base_ > 1024) {
        std::cout << "STACK OVERFLOW" << std::endl;
        // This exact line will not throw but just cause Segfault
        throw std::runtime_error("STACK OVERFLOW");
    }

    std::cout << global_current_instruction_idx_++ << " [";
    while (stack_ptr > global_base_) {
        std::cout << " " << *(--stack_ptr);
    }
    std::cout << " ]" << std::endl;
}

std::unique_ptr<Bytecode> CnpExecutionTrace::instrument(const Bytecode& bc) {
    auto result = std::vector<uint8_t>();
    auto trace_bytes = reinterpret_cast<uintptr_t>(&trace);
    const uint8_t debug_instruction[] = { static_cast<uint8_t>(Opcode::CALL_C_V_STACK_PTR), U64_TO_BYTES(trace_bytes) };
    std::size_t ip = 0;

    while (ip < bc.size()) {
        const auto op = static_cast<Opcode>(bc.data()[ip]);
        const auto step = OpcodeUtils::size(op);
        for (unsigned char trace_byte : debug_instruction) {
            result.push_back(trace_byte);
        }
        switch (op) {
        case Opcode::JUMP:
        case Opcode::JUMP_TRUE:
        case Opcode::JUMP_FALSE:
        case Opcode::FUNCTION_ADDRESS:
        case Opcode::CALL:
            {
                const auto fst_arg = *reinterpret_cast<const uint32_t*>(bc.data() + ip + 1);
                const auto doubled = fst_arg * 2;
                result.push_back(bc.data()[ip]);
                for (std::size_t j = 0; j < sizeof(uint32_t); j++) {
                    result.push_back(*(reinterpret_cast<const uint8_t*>(&doubled) + j));
                }
                break;
            }
        default:
            for (std::size_t j = 0; j < step; j++) {
                result.push_back(bc.data()[ip + j]);
            }
        }
        ip += step;
    }

    return std::make_unique<Bytecode>(std::move(result));
}
