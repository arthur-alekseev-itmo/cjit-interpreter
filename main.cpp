#include "core/Parsing/Bytecode/IO/BytecodeReader.h"

#include <cxxopts.hpp>
#include <iostream>
#include <loguru.hpp>

#include "core/BytecodeTransform/CnpExecutionTrace.h"
#include "core/Execution/CnpInterpreter.h"
#include "core/Parsing/Stencil/IO/CnpStencilFactory.h"
#include "core/Runtime/Runtime.h"

// TODO: Create an entity for this:
void configure_logging(const cxxopts::ParseResult &parse_result, int argc, char** argv) {
    loguru::init(argc, argv);
    loguru::g_preamble = false;

    const auto log_file =
        parse_result["log"].count()
        ? parse_result["log"].as<std::string>()
        : "log.log";

    if (parse_result["verbose"].count()) {
        loguru::add_file(log_file.c_str(), loguru::Truncate, loguru::Verbosity_MAX);
    } else {
        loguru::add_file(log_file.c_str(), loguru::Truncate, loguru::Verbosity_0);
    }
}

std::unique_ptr<CnpStencilCollection> parse_stencils(const cxxopts::ParseResult &parse_result) {
    const auto recompile = parse_result["recompile"].count()
        ? parse_result["recompile"].as<bool>()
        : false;

    auto stencils_factory = CnpStencilFactory();
    stencils_factory.set_recompile(recompile);

    if (parse_result["stencil-source"].count()) {
        stencils_factory.set_stencil_directory(parse_result["stencil-source"].as<std::string>());
    }

    if (parse_result["stencils"].count()) {
        stencils_factory.set_stencil_binary(parse_result["stencils"].as<std::string>());
    }

    return stencils_factory.create();
}

std::unique_ptr<BytecodeFile> parse_bytecode(const cxxopts::ParseResult &parse_result) {
    if (parse_result["input"].count() != 1) {
        std::cout << "Input must be provided via -i/--input";
        throw std::invalid_argument("Bad input source");
    }

    return BytecodeReader::read_file(parse_result["input"].as<std::string>());
}

void configure_trace(const cxxopts::ParseResult &parse_result, BytecodeFile &bytecode_file) {
    const auto trace = parse_result["trace"].count()
        ? parse_result["trace"].as<bool>()
        : false;

    if (trace) {
        CnpExecutionTrace::instrument(bytecode_file);
    }
}

int main(int argc, char** argv) {
    cxxopts::Options options("CangJit-Interpreter", "Interpreter for Cangjie Bytecode");

    options.add_options()
        ("t,trace", "Enable stack tracing after each instruction via instrumentation", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
        ("v,verbose", "Verbose logging of c&p process and instruction addresses", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
        ("i,input", "Input .cjbc file location", cxxopts::value<std::string>())
        ("r,recompile", "Require stencil recompilation, even if .o file exists", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
        ("s,stencils", "Stencils object file location", cxxopts::value<std::string>()->default_value("./stencils.o"))
        ("c,stencil-source", "Stencils .c file location", cxxopts::value<std::string>()->default_value("./stencils/stencils.c"))
        ("l,log", "Log output, default is log.log", cxxopts::value<std::string>());

    const auto result = options.parse(argc, argv);

    configure_logging(result, argc, argv);
    const auto stencils = parse_stencils(result);
    const auto bytecode_file = parse_bytecode(result);
    configure_trace(result, *bytecode_file.get());
    const auto codegen_result = CnpCodegen::compile(bytecode_file->get_bytecode(), stencils.get());
    const auto configured_runtime = Runtime::build(bytecode_file.get(), &codegen_result);

    CnpInterpreter::execute(codegen_result, configured_runtime);
}
