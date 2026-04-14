
#include <iostream>
#include <vector>

#include "bytecode/Instruction.h"
#include "bytecode/examples/BytecodeExamples.h"
#include "bytecode/io/BytecodeReader.h"
#include "cnp/execution/CnpInterpreter.h"
#include "cnp/stencil/CnpStencilFactory.h"

int main(int argc, char** argv) {
    auto stencil_factory = CnpStencilFactory();
    // stencil_factory.set_recompile(true);
    const auto interpreter = CnpInterpreter(stencil_factory.create());
    const auto bytecode = BytecodeReader::read_file("out.cjbc");
    interpreter.execute(*bytecode);
    return 0;
}
