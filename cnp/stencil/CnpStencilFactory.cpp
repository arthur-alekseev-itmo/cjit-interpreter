
#include "CnpStencilFactory.h"

#include <memory>
#include <fstream>
#include <LIEF/LIEF.hpp>

#include "../../ir/ir.hpp"
#include "../../utils/utils.hpp"


#define STENCIL_COUNT NUM_OPCODES
#define CNP_VALUE_HOLE_NAME "cnp_value_hole"
#define CNP_STENCIL_OUTPUT "cnp_func_hole"
#define JUMP_INSTRUCTION_SIZE 6

namespace
{
    CnpStencil parse_stencil(const LIEF::ELF::Binary* binary, const LIEF::Symbol* symbol)
    {
        if (symbol == nullptr) {
            throw std::runtime_error("Cannot find a stencil");
        }

        const auto section = binary->get_section(".text");

        const auto jump_instr_len = symbol->name() == "st_return" ? 0 : JUMP_INSTRUCTION_SIZE;
        const auto stencil_size = symbol->size() - jump_instr_len;
        const auto stencil_offset = symbol->value();

        auto code = std::vector<uint8_t>(stencil_size);
        auto patches = std::vector<CnpStencilPatch>();

        std::memcpy(
            code.data(),
            section->content().data() + stencil_offset,
            stencil_size
        );

        const auto relocations = binary->relocations();

        for (const LIEF::ELF::Relocation& relocation : relocations) {
            const auto address = relocation.address();

            if (!relocation.has_section() || relocation.section()->name() != ".text") {
                continue;
            }

            if (address < symbol->value() || address >= symbol->value() + stencil_size) {
                continue;
            }

            const auto type = relocation.type();
            const auto offset = address - symbol->value();
            patches.emplace_back(
                static_cast<LIEF::ELF::RELOC_x86_64>(type),
                offset
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

    // TODO: Remove: there is a way to generate stencils with jump
    void fill_jump_stencils(std::vector<CnpStencil>& stencils) {
#if defined(__arm__)
        throw std::runtime_error("Architecture __arm__ not yet supported");
#elif defined(__aarch64__)
        // TODO !!
        throw std::runtime_error("Architecture __aarch64__ not yet supported");
#elif defined(__x86_64__)
        stencils[JUMP] = CnpStencil(
            "jump",
            std::vector<uint8_t>({0xe9, 0x00, 0x00, 0x00, 0x00}),
            // TODO: Super sketchy and hacky!
            std::vector(1, CnpStencilPatch(LIEF::ELF::RELOC_x86_64::R_X86_64_GOTPCRELX, 1))
        );
#else
        throw std::runtime_error("Unknown architecture");
#endif
    }

    std::vector<CnpStencil> create_stencils(const std::string& stencil_obj) {
        auto stencils = std::vector<CnpStencil>(STENCIL_COUNT);
        const auto binary = LIEF::ELF::Parser::parse(stencil_obj);

        #define PARSE_STENCIL(opcode, name) stencils[opcode] = parse_stencil(binary.get(), binary->get_symbol(name));

        PARSE_STENCIL(NOP, "st_nop");
        PARSE_STENCIL(LOAD_IMM, "st_load_imm");
        PARSE_STENCIL(ADD, "st_add");
        PARSE_STENCIL(SUB, "st_sub");
        PARSE_STENCIL(MUL, "st_mul");
        PARSE_STENCIL(CALL_C_V_U64, "st_call_c_u64");
        PARSE_STENCIL(CALL_C_V_STACK_PTR, "st_call_c_stack_ptr");
        PARSE_STENCIL(EXIT, "st_exit");
        PARSE_STENCIL(DUP, "st_dup");
        PARSE_STENCIL(DROP, "st_drop");
        PARSE_STENCIL(RETURN, "st_return");
        PARSE_STENCIL(READ_STACK, "st_read_stack");
        PARSE_STENCIL(WRITE_STACK, "st_write_stack");
        PARSE_STENCIL(JUMP_TRUE, "st_jump_true");
        PARSE_STENCIL(EQ, "st_eq");
        PARSE_STENCIL(CALL, "st_call");
        PARSE_STENCIL(SWAP, "st_swap");

        fill_jump_stencils(stencils);

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
