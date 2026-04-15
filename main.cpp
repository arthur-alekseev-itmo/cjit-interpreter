#include "core/bytecode/io/BytecodeReader.h"

#include <cxxopts.hpp>
#include <iostream>
#include <loguru.hpp>

#include "core/cnp/execution/CnpInterpreter.h"
#include "core/cnp/stencil/CnpStencilFactory.h"

int main(int argc, char** argv) {
    cxxopts::Options options("CangJit-Interpreter", "Interpreter for Cangjie Bytecode");

    options.add_options()
        ("t,trace", "Enable stack tracing after each instruction via instrumentation", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
        ("v,verbose", "Verbose logging of c&p process and instruction addresses", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
        ("i,input", "Input .cjbc file location", cxxopts::value<std::string>())
        ("r,recompile", "Require stencil recompilation, even if .o file exists", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
        ("l,log", "Log output, default is log.log", cxxopts::value<std::string>());

    const auto result = options.parse(argc, argv);

    loguru::init(argc, argv);

    const auto log_file =
        result["log"].count()
        ? result["log"].as<std::string>()
        : "log.log";

    if (result["verbose"].count()) {
        loguru::add_file(log_file.c_str(), loguru::Truncate, loguru::Verbosity_MAX);
    } else {
        loguru::add_file(log_file.c_str(), loguru::Truncate, loguru::Verbosity_0);
    }

    if (result["input"].count() != 1) {
        std::cout << "Input must be provided via -i/--input";
        return 1;
    }

    const auto bytecode = BytecodeReader::read_file(result["input"].as<std::string>());

    const auto recompile = result["recompile"].count()
        ? result["recompile"].as<bool>()
        : false;
    auto stencils_factory = CnpStencilFactory();
    stencils_factory.set_recompile(recompile);

    auto interpreter = CnpInterpreter(stencils_factory.create());
    const auto trace = result["trace"].count()
        ? result["trace"].as<bool>()
        : false;
    interpreter.set_instrument(trace);
    interpreter.execute(*bytecode);
}
