#include <cassert>
#include <cstdio>
#include <cstring>


#include "interpreter.hpp"

#include <iomanip>
#include <iostream>

#include "../ir/ir.hpp"
#include <sys/mman.h>
#include "stencils.hpp"

#define ADVANCE(size) ip += size; break

void copy_patched(
    const stencil* stencil,
    uint8_t* target,
    std::size_t& target_offset,
    const std::initializer_list<const uint32_t*> patches
) {
    const auto stencil_size = stencil->code->size();
    uint8_t* target_addr = &target[target_offset];

    std::memcpy(
        target_addr,
        stencil->code->data(),
        stencil_size
    );

    auto patch_index = 0;
    for (const auto patch : patches) {
        const auto patch_address = stencil->patches_addresses->at(patch_index);
        const auto* data = reinterpret_cast<const uint8_t*>(patch);

        std::memcpy(
            target_addr + patch_address,
            data,
            sizeof(uint32_t)
        );

        patch_index++;
    }

    target_offset += stencil_size;
}

const void* printf_addr = reinterpret_cast<void*>(printf);

void apply_bytecode_instruction(
    const bytecode_ptr bc_slice,
    std::size_t& ip,
    std::size_t& target_offset,
    uint8_t* target,
    const stencil* stencils,
    const std::vector<std::size_t>& jumps
) {
    const auto opcode = bc_slice[ip];
    switch (opcode) {
        case NOP:
            ADVANCE(1);
        case LOAD_IMM:
            copy_patched(&stencils[opcode], target, target_offset,
                {reinterpret_cast<const uint32_t*>(&bc_slice[ip + 1])}
            );
            ADVANCE(5);
        case PRINT:
            copy_patched(&stencils[opcode], target, target_offset,
                {static_cast<const uint32_t*>(printf_addr)}
            );
            ADVANCE(1);
        case MUL:
        case ADD:
        case EXIT:
        case DUP:
        case RETURN:
            copy_patched(&stencils[opcode], target, target_offset, {});
            ADVANCE(1);
        case JUMP:
            {
                const auto jump_addr_index = *reinterpret_cast<const uint32_t*>(&bc_slice[ip + 1]);
                const auto jump_addr = jumps[jump_addr_index];
                const auto relative_jump = jump_addr - target_offset;
                copy_patched(&stencils[opcode], target, target_offset,
                    {reinterpret_cast<const uint32_t*>(&relative_jump)}
                );
                ADVANCE(5);
            }
        default:
            // TODO:
            assert(false);
    }
}

void print_hex_bytes(const std::vector<uint8_t>& data) {
    std::cout << std::hex << std::setfill('0');
    for (const uint8_t byte : data) {
        std::cout << std::setw(2) << static_cast<unsigned int>(byte) << " ";
    }
    std::cout << std::dec << std::endl; // Reset to decimal for subsequent prints
}

// TODO: Fix possible leaks
jit_function create_jit_function(const bytecode& bc, stencil* stencils) {
    const auto code = static_cast<uint8_t*>(mmap(
        nullptr,
        256,
        PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    ));
    assert (code != MAP_FAILED);

    const auto result = reinterpret_cast<jit_function>(code);
    auto jump_addresses = std::vector<std::size_t>(0);

    std::size_t target_offset = 0;

    for (std::size_t ip = 0; ip < bc.size();) {
        jump_addresses.push_back(target_offset);
        apply_bytecode_instruction(
            bc.data(),
            ip,
            target_offset,
            code,
            stencils,
            jump_addresses
        );
    }

    // Debug stuff
    const auto debug_view = std::vector<uint8_t>(code, code + target_offset);
    print_hex_bytes(debug_view);
    // End Debug stuff


    const auto rc = mprotect(
        reinterpret_cast<void*>(result),
        256,
        PROT_READ | PROT_EXEC
    );

    if (rc) {
        perror("Failed to create executable memory (m protect)");
    }

    return result;
}

void interpret_cnp(const bytecode& code, stencil* stencils) {
    const auto fn = create_jit_function(code, stencils);
    int64_t stack[0x10] = {};
    int64_t* stack_top = stack;
    fn(stack_top);
    std::cout << *stack_top;
}