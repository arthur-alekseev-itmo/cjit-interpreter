
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

    int ensure_stencil_file(const std::string& sources, const std::string& output, const bool recompile) {
        const auto output_file = std::ifstream(output);
        if (!recompile && output_file) {
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

        #define DEF_INSTR(opcode, name, size, stencil, trim) stencils[static_cast<std::size_t>(Opcode::opcode)] = parse_stencil(binary.get(), binary->get_symbol(stencil), trim);
        #include "../../bytecode/Instructions.def"
        #undef DEF_INSTR

        return stencils;
    }
}

std::unique_ptr<CnpStencilCollection> CnpStencilFactory::create() const {
    ensure_stencil_file(
        this->stencil_path_,
        this->stencil_binary_path_,
        recompile_
    );
    const auto stencils = create_stencils(this->stencil_binary_path_);
    return std::make_unique<CnpStencilCollection>(stencils);
}

CnpStencilFactory::CnpStencilFactory() {
    this->stencil_path_ = "stencils/stencils.c";
    this->stencil_binary_path_ = "stencils.o";
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
    this->recompile_ = recompile;
    return this;
}
