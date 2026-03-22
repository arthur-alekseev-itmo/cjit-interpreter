#include "CnpCodegen.h"

#include <cassert>

#define ADVANCE(size) ip += size; break
#define JUMP_SIZE 5
#define EXTRACT_ARGUMENT_VALUE(type, offset) (&bc_slice[ip + (offset)])
#define EXTRACT_ARGUMENT_PTR(type, offset) CnpPatchValue(sizeof(type), reinterpret_cast<const uint8_t*>(&bc_slice[ip + (offset)]))
#define COPY_AND_PATCH(...) (*stencils)[op].patch(function_ptr, function_offset, data_ptr, data_offset, {__VA_ARGS__})

namespace {
    std::size_t relocation_outer_size(const LIEF::ELF::RELOC_x86_64 type) {
        // TODO: Take info from here if needed:
        // https://docs.oracle.com/cd/E19120-01/open.solaris/819-0690/chapter7-2/index.html
        // TODO!!!!
        return 8;
    }

    struct function_layout {
        std::size_t data_section_size;
        std::size_t instructions_size;
        std::vector<std::size_t> instruction_starts;
    };

    function_layout calculate_layout(bytecode& bc, CnpStencilCollection* stencils) {
        std::size_t instruction_size = 0;
        std::size_t data_section_size = 0;
        std::size_t ip = 0;
        auto instruction_starts = std::vector<std::size_t>();

        const std::size_t bc_size = bc.size();
        while (ip < bc_size) {
            const auto op = static_cast<opcode>(bc[ip]);
            const auto& stencil = (*stencils)[op];
            for (const auto patch : stencil.patches) {
                data_section_size += relocation_outer_size(patch.type);
            }
            instruction_starts.push_back(instruction_size);
            instruction_size += stencil.size();
            ip += opcode_size(op);
        }
        return {data_section_size, instruction_size, instruction_starts};
    }

    void apply_bytecode_instruction(
        const bytecode_ptr bc_slice,
        std::size_t& ip,
        uint8_t* function_ptr,
        std::size_t& function_offset,
        uint8_t* data_ptr,
        std::size_t& data_offset,
        const CnpStencilCollection* stencils,
        const std::vector<std::size_t>& jumps
    ) {
        const auto op = static_cast<opcode>(bc_slice[ip]);
        switch (op) {
        case NOP:
            ADVANCE(1);
        case READ_STACK:
        case WRITE_STACK:
            COPY_AND_PATCH(EXTRACT_ARGUMENT_PTR(uint32_t, 1));
            ADVANCE(5);
        case LOAD_IMM:
            COPY_AND_PATCH(EXTRACT_ARGUMENT_PTR(uint64_t, 1));
            ADVANCE(9);
        case CALL_C_V_STACK_PTR:
        case CALL_C_V_U64:
            COPY_AND_PATCH(CnpPatchValue(sizeof(intptr_t), EXTRACT_ARGUMENT_VALUE(intptr_t, 1)));
            ADVANCE(9);
        case MUL:
        case ADD:
        case SUB:
        case EXIT:
        case DUP:
        case DROP:
        case SWAP:
        case EQ:
        case RETURN:
            COPY_AND_PATCH();
            ADVANCE(1);
        case JUMP_TRUE:
        case JUMP:
        case CALL:
            {
                // Тяжело...
                // TODO: Somehow understand if the address will be relative or absolute.
                // Absolute for now
                const auto jump_addr_index = *reinterpret_cast<const uint32_t*>(&bc_slice[ip + 1]);
                const auto jump_addr = jumps[jump_addr_index];
                const auto relative_jump = jump_addr - function_offset - JUMP_SIZE;
                const auto relative_jump_patch_addr = reinterpret_cast<const uint8_t*>(&relative_jump);
                const auto absolute_jump = function_ptr + jump_addr;
                const auto absolute_jump_patch_addr = reinterpret_cast<const uint8_t*>(&absolute_jump);
                COPY_AND_PATCH(CnpPatchValue(sizeof(int64_t), absolute_jump_patch_addr ));
                ADVANCE(5);
            }
        default:
            // TODO:
            assert(false);
        }
    }

    void cnp_compile(
        uint8_t* code_ptr,
        uint8_t* data_ptr,
        const CnpStencilCollection* stencils,
        const bytecode& bc,
        const std::vector<std::size_t>& instruction_starts
    ) {
        std::size_t function_offset = 0;
        std::size_t data_offset = 0;

        for (std::size_t ip = 0; ip < bc.size();) {
            apply_bytecode_instruction(
                bc.data(),
                ip,
                code_ptr,
                function_offset,
                data_ptr,
                data_offset,
                stencils,
                instruction_starts
            );
        }
    }
}

CnpFunction CnpCodegen::compile(bytecode& bc, CnpStencilCollection* stencils)
{
    const auto [
        data_section_size,
        instructions_size,
        instruction_starts
    ] = calculate_layout(bc, stencils);

    const auto function = CnpFunction(data_section_size, instructions_size);

    cnp_compile(
        function.function_ptr(),
        function.data_ptr(),
        stencils,
        bc,
        instruction_starts
    );

    return function;
}
