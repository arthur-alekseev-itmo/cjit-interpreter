
#include "CnpStencilFactory.h"

#include <memory>
#include <fstream>
#include <LIEF/LIEF.hpp>

#include "../../utils/utils.hpp"
#include "../parsing/BinaryWrapper.hpp"
#include "../parsing/ElfBinaryWrapper.h"
#include "../parsing/MachOBinaryWrapper.hpp"
#include "../parsing/RelocationWrapper.h"
#include "../../bytecode/Opcode.h"


#define STENCIL_COUNT NUM_OPCODES
#define CNP_VALUE_HOLE_NAME "cnp_value_hole"
#define CNP_STENCIL_OUTPUT "cnp_func_hole"

#if defined(__arm__)
        throw std::runtime_error("Architecture __arm__ not yet supported");
#elif defined(__aarch64__)
    #define JUMP_INSTRUCTION_SIZE 4
#elif defined(__x86_64__)
    #define JUMP_INSTRUCTION_SIZE 6
#else
throw std::runtime_error("Unknown architecture");
#endif

namespace {
    CnpStencil parse_stencil(const BinaryWrapper* binary, const SymbolWrapper* symbol, const bool cut_jump) {
        if (symbol == nullptr) {
            throw std::runtime_error("Cannot find a stencil");
        }

        const auto section = binary->get_text_section();

        const auto jump_instr_len = cut_jump ? JUMP_INSTRUCTION_SIZE : 0;
        const auto stencil_size = symbol->size() - jump_instr_len;
        const auto stencil_offset = symbol->offset();

        auto code = std::vector<uint8_t>(stencil_size);
        auto patches = std::vector<CnpStencilPatch>();

        std::memcpy(
            code.data(),
            section->content().data() + stencil_offset,
            stencil_size
        );

        const auto relocations = binary->get_relocations_for_symbol(symbol, cut_jump);

        for (const RelocationWrapper& relocation : relocations) {
            const auto address = relocation.address;
            patches.emplace_back(
                relocation.type,
                address - symbol->offset(),
                relocation.architecture
            );
        }

        return {symbol->name(), std::move(code), std::move(patches)};
    }

    int ensure_stencil_file(const std::string& sources, const std::string& output) {
        if (const std::ifstream output_file(output); output_file) {
            // TODO: Check modification dates like make (or use make)
            return 0;
        }
        const std::string flags = "-Os -fPIC -fno-plt -fno-stack-protector -fno-asynchronous-unwind-tables";
        const std::string stencil_compile_cmd = "clang " + flags + " -c " + sources + " -o " + output;
        return exec_silent(stencil_compile_cmd.c_str());
    }

    std::vector<CnpStencil> create_stencils(const std::string& stencil_obj) {
        auto stencils = std::vector<CnpStencil>(OpcodeUtils::opcode_count());
#if defined(__aarch64__)
        const auto binary = std::make_unique<MachOBinaryWrapper>(stencil_obj);
#elif defined(__x86_64__)
        const auto binary = std::make_unique<ElfBinaryWrapper>(stencil_obj);
#else
        throw std::runtime_error("Unknown architecture");
#endif

        #define PARSE_STENCIL(opcode, name) stencils[static_cast<std::size_t>(opcode)] = parse_stencil(binary.get(), binary->get_symbol(name), true);
        #define PARSE_STENCIL_FULL(opcode, name) stencils[static_cast<std::size_t>(opcode)] = parse_stencil(binary.get(), binary->get_symbol(name), false);

        PARSE_STENCIL(Opcode::NOP, "st_nop");
        PARSE_STENCIL(Opcode::LOAD_IMM, "st_load_imm");
        PARSE_STENCIL(Opcode::ADD, "st_add");
        PARSE_STENCIL(Opcode::SUB, "st_sub");
        PARSE_STENCIL(Opcode::MUL, "st_mul");
        PARSE_STENCIL(Opcode::CALL_C_V_U64, "st_call_c_u64");
        PARSE_STENCIL(Opcode::CALL_C_V_STACK_PTR, "st_call_c_stack_ptr");
        PARSE_STENCIL(Opcode::EXIT, "st_exit");
        PARSE_STENCIL(Opcode::DUP, "st_dup");
        PARSE_STENCIL(Opcode::DROP, "st_drop");
        PARSE_STENCIL(Opcode::READ_STACK, "st_read_stack");
        PARSE_STENCIL(Opcode::WRITE_STACK, "st_write_stack");
        PARSE_STENCIL(Opcode::JUMP_TRUE, "st_jump_true");
        PARSE_STENCIL(Opcode::EQ, "st_eq");
        PARSE_STENCIL(Opcode::CALL, "st_call");
        PARSE_STENCIL(Opcode::SWAP, "st_swap");
        PARSE_STENCIL(Opcode::CALL_BUILTIN, "st_call_builtin");

        PARSE_STENCIL_FULL(Opcode::RETURN, "st_return");
        PARSE_STENCIL_FULL(Opcode::JUMP, "st_jump");

        return stencils;
    }
}

std::unique_ptr<CnpStencilCollection> CnpStencilFactory::create() const {
    // TODO: Uncomment, currently hangs in debugger
    ensure_stencil_file(this->stencil_path_, this->stencil_binary_path_);
    const auto stencils = create_stencils(this->stencil_binary_path_);
    return std::make_unique<CnpStencilCollection>(stencils);
}

CnpStencilFactory::CnpStencilFactory() {
    this->stencil_path_ = "stencils/basic_stencils.c";
    this->stencil_binary_path_ = "basic_stencils.o";
    this->recompile_ = false;
}

CnpStencilFactory* CnpStencilFactory::set_stencil_directory(const std::string& path) {
    this->stencil_path_ = path;
    return this;
}

CnpStencilFactory* CnpStencilFactory::set_stencil_binary(const std::string& path) {
    this->stencil_binary_path_ = path;
    return this;
}

CnpStencilFactory* CnpStencilFactory::set_recompile(const bool recompile) {
    // TODO: Use!
    this->recompile_ = recompile;
    return this;
}
