
#include <iostream>
#include <vector>

#include "bytecode/Instruction.h"
#include "bytecode/examples/BytecodeExamples.h"
#include "bytecode/io/BytecodeReader.h"
#include "cnp/execution/CnpInterpreter.h"
#include "cnp/stencil/CnpStencilFactory.h"

int main(int argc, char** argv) {
    auto stencil_factory = CnpStencilFactory();
    stencil_factory.set_recompile(true);
    const auto interpreter = CnpInterpreter(stencil_factory.create());
    const auto sampleBc = BytecodeReader::read_file("/Users/arthur/Documents/cj2bc/even.cjbc");
    auto sample = InstructionList(sampleBc.get());
    std::cout << sample << std::endl;
    const auto bytecode = Bytecode(print_30_fib, std::size(print_30_fib));
    interpreter.execute(bytecode);
    return 0;
}
