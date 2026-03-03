#include "stencils.hpp"

#include <memory>
#include <iostream>
#include <fstream>
#include <LIEF/LIEF.hpp>

#include "../ir/ir.hpp"
#include "../utils/utils.hpp"


#define STENCIL_COUNT 9

#define CNP_VALUE_HOLE_NAME "cnp_value_hole"
#define CNP_STENCIL_OUTPUT "cnp_func_hole"
#define JUMP_INSTRUCTION_SIZE 5

stencil parse_stencil(const LIEF::ELF::Binary* binary, const LIEF::Symbol* symbol)
{
    if (symbol == nullptr) {
        throw std::runtime_error("Cannot find a stencil");
    }

    const auto section = binary->get_section(".text");

    const auto jump_instr_len = symbol->name() == "st_return" ? 0 : JUMP_INSTRUCTION_SIZE;
    const auto stencil_size = symbol->size() - jump_instr_len;
    const auto stencil_offset = symbol->value();

    auto code = std::make_unique<std::vector<uint8_t>>(stencil_size);
    auto patch_addresses = std::make_unique<std::vector<std::size_t>>();

    std::memcpy(
        code->data(),
        section->content().data() + stencil_offset,
        stencil_size
    );

    const auto relocations = binary->relocations();

    for (const auto& relocation : relocations) {
        const auto address = relocation.address();

        if (address < symbol->value() || address >= symbol->value() + stencil_size) {
            continue;
        }

        // TODO: Function patch addresses in another struct field
        // if (relocation.symbol()->name().contains(CNP_VALUE_HOLE_NAME)) {
        //     patch_addresses->push_back(address - symbol->value());
        // }

        patch_addresses->push_back(address - symbol->value());
    }

    return {std::move(code), std::move(patch_addresses)};
}

int ensure_stencil_file(const std::string& sources, const std::string& output) {
    if (const std::ifstream output_file(output); output_file) {
        // TODO: Check modification dates like make (or use make)
        return 0;
    }
    const std::string stencil_compile_cmd = "clang -O3 -fno-pic -c " + sources + " -o " + output;
    return exec_silent(stencil_compile_cmd.c_str());
}

void fill_jump_stencils(std::vector<stencil>* stencils) {
#if defined(__arm__)
    throw std::runtime_error("Architecture __arm__ not yet supported");
#elif defined(__aarch64__)
    // TODO !!
    throw std::runtime_error("Architecture __aarch64__ not yet supported");
#elif defined(__x86_64__)
    (*stencils)[JUMP] = stencil(
        std::make_unique<std::vector<uint8_t>>(std::vector<uint8_t>({0xe9, 0x00, 0x00, 0x00, 0x00})),
        std::make_unique<std::vector<size_t>>(std::vector<size_t>({1}))
    );
#else
    throw std::runtime_error("Unknown architecture");
#endif
}

std::unique_ptr<std::vector<stencil>> create_stencils()
{
    const std::string stencil_location = "stencils/basic_stencils.c";
    const std::string stencil_obj = "basic_stencils.o";

    ensure_stencil_file(stencil_location, stencil_obj);

    auto stencils = std::make_unique<std::vector<stencil>>(STENCIL_COUNT);
    const auto binary = LIEF::ELF::Parser::parse(stencil_obj);

    #define PARSE_STENCIL(opcode, name) (*stencils)[opcode] = parse_stencil(binary.get(), binary->get_symbol(name));

    PARSE_STENCIL(NOP, "st_nop");
    PARSE_STENCIL(LOAD_IMM, "st_load_imm");
    PARSE_STENCIL(ADD, "st_add");
    PARSE_STENCIL(MUL, "st_mul");
    PARSE_STENCIL(PRINT, "st_print");
    PARSE_STENCIL(EXIT, "st_exit");
    PARSE_STENCIL(DUP, "st_dup");
    PARSE_STENCIL(RETURN, "st_return");

    fill_jump_stencils(stencils.get());

    return stencils;
}