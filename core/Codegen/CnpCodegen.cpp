#include "CnpCodegen.h"

#include <cassert>

#include "LIEF/ELF/enums.hpp"
#include "LIEF/MachO/enums.hpp"
#include "loguru.hpp"

#include "../Parsing/Bytecode/Data/Instruction.h"
#include "../Parsing/Bytecode/Data/Opcode.h"
#include "CnpPatcher.h"


#define ADVANCE ip += (OpcodeUtils::size(op)); break
#define EXTRACT_ARGUMENT_VALUE(type, offset) (&bc_slice[ip + (offset)])
#define EXTRACT_ARGUMENT_PTR(type, offset) CnpPatchValue(sizeof(type), reinterpret_cast<const uint8_t*>(&bc_slice[ip + (offset)]))
#define COPY_AND_PATCH(...) CnpPatcher::patch((*stencils)[static_cast<std::size_t>(op)], function_ptr, function_offset, data_ptr, data_offset, {__VA_ARGS__})

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

    std::size_t relocation_outer_size(CnpStencilPatch patch) {
        const auto t = patch.architecture;
        if (t == 12 /* AARCH64 (TODO: Find the type, dont use magic numbers) */) {
            switch (static_cast<LIEF::MachO::ARM64_RELOCATION>(patch.type)) {
                case LIEF::MachO::ARM64_RELOCATION::ARM64_RELOC_PAGE21: return 8;
                default: return 0;
            }
        }
        if (t == 62 /* X86_64 (TODO: Find the type, dont use magic numbers) */) {
            switch (static_cast<LIEF::ELF::RELOC_x86_64>(patch.type)) {
            case LIEF::ELF::RELOC_x86_64::R_X86_64_GOTPCRELX:
            case LIEF::ELF::RELOC_x86_64::R_X86_64_REX_GOTPCRELX:
            case LIEF::ELF::RELOC_x86_64::R_X86_64_PC32:
                return 8;
            default: return 0;
            }
        }
        throw std::runtime_error("TODO!!");
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
                data_section_size += relocation_outer_size(patch);
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
        case Opcode::STORE_LOCAL:
        case Opcode::STORE_GLOBAL:
        case Opcode::STORE_CLOSURE:
        case Opcode::LOAD_LOCAL:
        case Opcode::LOAD_GLOBAL:
        case Opcode::LOAD_CLOSURE:
        case Opcode::GET_LOCAL_REF:
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
        case Opcode::DIV:
        case Opcode::MOD:
        case Opcode::GE:
        case Opcode::GT:
        case Opcode::LE:
        case Opcode::LT:
        case Opcode::EQ:
        case Opcode::NEQ:
        case Opcode::NOT:
        case Opcode::NEG:
        case Opcode::AND:
        case Opcode::OR:
        case Opcode::BITAND:
        case Opcode::BITOR:
        case Opcode::BITXOR:
        case Opcode::EXIT:
        case Opcode::DUP:
        case Opcode::DROP:
        case Opcode::SWAP:
        case Opcode::RETURN:
        case Opcode::CALL_OBJECT:
        case Opcode::WRITE_TO_REF:
            COPY_AND_PATCH();
            ADVANCE;
        case Opcode::JUMP_TRUE:
        case Opcode::JUMP_FALSE:
        case Opcode::JUMP:
        case Opcode::CALL:
        case Opcode::FUNCTION_ADDRESS:
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
        // default:
        //     // TODO:
        //     throw std::runtime_error("Opcode to be supported: " + OpcodeUtils::to_string(op));
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

    auto il = InstructionList(bc);
    std::size_t i = 0;
    for (const auto& instruction : il) {
        VLOG_S(1) << i << " " << reinterpret_cast<void*>(function.function_ptr() + instruction_starts[i++]) << " " << instruction;
    }

    cnp_compile(
        function.function_ptr(),
        function.data_ptr(),
        stencils,
        bc,
        instruction_starts
    );

    return function;
}
