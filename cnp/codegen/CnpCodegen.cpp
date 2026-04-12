#include "CnpCodegen.h"

#include <cassert>

#include "../../builtins/Builtins.h"
#include "../../bytecode/Opcode.h"

#define ADVANCE ip += (OpcodeUtils::size(op)); break
#define EXTRACT_ARGUMENT_VALUE(type, offset) (&bc_slice[ip + (offset)])
#define EXTRACT_ARGUMENT_PTR(type, offset) CnpPatchValue(sizeof(type), reinterpret_cast<const uint8_t*>(&bc_slice[ip + (offset)]))
#define COPY_AND_PATCH(...) (*stencils)[static_cast<std::size_t>(op)].patch(function_ptr, function_offset, data_ptr, data_offset, {__VA_ARGS__})

#if defined(__aarch64__)
    #define JUMP_SIZE 0
#elif defined(__x86_64__)
    #define JUMP_SIZE 5
#endif


namespace {
    bool is_relative_jump(CnpStencilPatch patch) {
        const auto t = patch.architecture;
        if (t == 12 /* AARCH64 (TODO: Find the type, dont use magic numbers) */)
            return patch.type == static_cast<uint32_t>(LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_BRANCH26);
        return false;
    }

    std::size_t relocation_outer_size(uint32_t type) {
        // TODO: Take info from here if needed:
        // https://docs.oracle.com/cd/E191 20-01/open.solaris/819-0690/chapter7-2/index.html
        // TODO!!!!
        return 8;
    }

    struct function_layout {
        std::size_t data_section_size;
        std::size_t instructions_size;
        std::vector<std::size_t> instruction_starts;
    };

    function_layout calculate_layout(const Bytecode& bc, CnpStencilCollection* stencils) {
        std::size_t instruction_size = 0;
        std::size_t data_section_size = 0;
        std::size_t ip = 0;
        auto instruction_starts = std::vector<std::size_t>();

        const std::size_t bc_size = bc.size();
        while (ip < bc_size) {
            const auto op = static_cast<Opcode>(bc.data()[ip]);
            const auto& stencil = (*stencils)[static_cast<std::size_t>(op)];
            for (const auto patch : stencil.patches) {
                data_section_size += relocation_outer_size(patch.type);
            }
            instruction_starts.push_back(instruction_size);
            instruction_size += stencil.size();
            ip += OpcodeUtils::size(op);
        }
        return {data_section_size, instruction_size, instruction_starts};
    }

    void apply_bytecode_instruction(
        const uint8_t* bc_slice,
        std::size_t& ip,
        uint8_t* function_ptr,
        std::size_t& function_offset,
        uint8_t* data_ptr,
        std::size_t& data_offset,
        const CnpStencilCollection* stencils,
        const std::vector<std::size_t>& jumps
    ) {
        const auto op = static_cast<Opcode>(bc_slice[ip]);
        switch (op) {
        case Opcode::NOP: ADVANCE;
        case Opcode::READ_STACK:
        case Opcode::WRITE_STACK:
            COPY_AND_PATCH(EXTRACT_ARGUMENT_PTR(uint32_t, 1));
            ADVANCE;
        case Opcode::LOAD_IMM:
            COPY_AND_PATCH(EXTRACT_ARGUMENT_PTR(uint64_t, 1));
            ADVANCE;
        case Opcode::CALL_C_V_STACK_PTR:
        case Opcode::CALL_C_V_U64:
            COPY_AND_PATCH(CnpPatchValue(sizeof(intptr_t), EXTRACT_ARGUMENT_VALUE(intptr_t, 1)));
            ADVANCE;
        case Opcode::MUL:
        case Opcode::ADD:
        case Opcode::SUB:
        case Opcode::EXIT:
        case Opcode::DUP:
        case Opcode::DROP:
        case Opcode::SWAP:
        case Opcode::EQ:
        case Opcode::RETURN:
            COPY_AND_PATCH();
            ADVANCE;
        case Opcode::JUMP_TRUE:
        case Opcode::JUMP:
        case Opcode::CALL:
            {
                const auto jump_addr_index = *reinterpret_cast<const uint32_t*>(&bc_slice[ip + 1]);
                const auto jump_addr = jumps[jump_addr_index];
                const uint64_t relative_jump = jump_addr - function_offset - JUMP_SIZE;
                const auto relative_jump_patch_addr = reinterpret_cast<const uint8_t*>(&relative_jump);
                const auto absolute_jump = function_ptr + jump_addr;
                const auto absolute_jump_patch_addr = reinterpret_cast<const uint8_t*>(&absolute_jump);

                if (is_relative_jump(stencils->operator[](static_cast<std::size_t>(op)).patches[0])) {
                    // Relative
                    COPY_AND_PATCH(CnpPatchValue(sizeof(uint64_t), relative_jump_patch_addr ));
                } else {
                    // Absolute
                    COPY_AND_PATCH(CnpPatchValue(sizeof(uint64_t), absolute_jump_patch_addr ));
                }
                ADVANCE;
            }
        case Opcode::CALL_BUILTIN:
            {
                const auto builtin_index = *reinterpret_cast<const uint32_t*>(&bc_slice[ip + 1]);
                assert(builtin_index < std::size(builtin_addresses));
                const auto fn_address = builtin_addresses[builtin_index];
                COPY_AND_PATCH(CnpPatchValue(sizeof(intptr_t), reinterpret_cast<const uint8_t*>(&fn_address)));
                ADVANCE;
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
        const Bytecode& bc,
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

CnpFunction CnpCodegen::compile(const Bytecode& bc, CnpStencilCollection* stencils)
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
