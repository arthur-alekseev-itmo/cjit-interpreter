#include "CnpFunction.h"

#include <cassert>
#include <iomanip>
#include <sys/mman.h>

#define ADVANCE(size) ip += size; break

namespace
{
    // TODO: Create a separate JIT-Compiler entity to create functions
    void apply_bytecode_instruction(
        const bytecode_ptr bc_slice,
        std::size_t& ip,
        std::size_t& target_offset,
        uint8_t* target,
        const CnpStencilCollection& stencils,
        const std::vector<std::size_t>& jumps
    ) {
        const auto opcode = bc_slice[ip];
        switch (opcode) {
        case NOP:
            ADVANCE(1);
        case LOAD_IMM:
            stencils[opcode].patch(
                target,
                target_offset,
                {reinterpret_cast<const uint32_t*>(&bc_slice[ip + 1])}
            );
            ADVANCE(5);
        case CALL_C_V_U64:
            {
                // TODO: Make a load system for functions
                const auto call_address = reinterpret_cast<uintptr_t>(&my_print);
                const uint32_t call_address_left = call_address >> 32;
                const uint32_t call_address_right = call_address & 0xffffffff;
                stencils[opcode].patch(
                    target,
                    target_offset,
                    {&call_address_left, &call_address_right}
                );
                ADVANCE(9);
            }
        case MUL:
        case ADD:
        case EXIT:
        case DUP:
        case RETURN:
            stencils[opcode].patch(target, target_offset, {});
            ADVANCE(1);
        case JUMP:
            {
                const auto jump_addr_index = *reinterpret_cast<const uint32_t*>(&bc_slice[ip + 1]);
                const auto jump_addr = jumps[jump_addr_index];
                const auto relative_jump = jump_addr - target_offset;
                stencils[opcode].patch(
                    target,
                    target_offset,
                    {reinterpret_cast<const uint32_t*>(&relative_jump)}
                );
                ADVANCE(5);
            }
        default:
            // TODO:
            assert(false);
        }
    }

    std::pair<std::size_t, std::vector<std::size_t>> instruction_layout(
        const CnpStencilCollection& stencils,
        const bytecode& bc
    ) {
        std::size_t total = 0;
        auto instruction_starts = std::vector<std::size_t>();

        for (std::size_t ip = 0; ip < bc.size();) {
            const auto current = static_cast<opcode>(bc[ip]);
            const auto size = opcode_size(current);
            instruction_starts.push_back(total);
            total += stencils[current].size();
            ip += size;
        }

        return {total, instruction_starts};
    }

    std::pair<cnp_function_ptr, std::size_t> create_jit_function(
        const CnpStencilCollection& stencils,
        const bytecode& bc
    ) {
        const auto [
            function_size,
            instruction_starts
        ] = instruction_layout(stencils, bc);

        const auto code = static_cast<uint8_t*>(mmap(
            nullptr,
            function_size,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        ));

        assert (code != MAP_FAILED);

        const auto result = reinterpret_cast<cnp_function_ptr>(code);
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

        const auto rc = mprotect(
            reinterpret_cast<void*>(result),
            function_size,
            PROT_READ | PROT_EXEC
        );

        if (rc) {
            perror("Failed to create executable memory (m protect)");
        }

        return {result, function_size};
    }
}


std::size_t* CnpFunction::call(std::size_t* stack_top) const {
    return this->get_function_ptr()(stack_top);
}

cnp_function_ptr CnpFunction::get_function_ptr() const {
    return this->function_ptr_;
}

CnpFunction::CnpFunction(const CnpStencilCollection& stencils, bytecode& bc) {
    const auto [
        function_ptr,
        function_size
    ] = create_jit_function(stencils, bc);

    this->function_size_ = function_size;
    this->function_ptr_ = function_ptr;
}

// TODO: Maybe free function pte?
CnpFunction::~CnpFunction() = default;


std::ostream& operator<<(std::ostream& os, const CnpFunction& fn)
{
    std::cout << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < fn.function_size_; i++) {
        const auto byte = reinterpret_cast<uint8_t*>(fn.function_ptr_)[i];
        std::cout << std::setw(2) << static_cast<unsigned int>(byte) << " ";
    }
    std::cout << std::dec << std::endl; // Reset to decimal for subsequent prints
    return os;
}
