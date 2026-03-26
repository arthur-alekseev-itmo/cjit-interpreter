
#include <vector>

#include "bytecode/examples/BytecodeExamples.h"
#include "cnp/execution/CnpInterpreter.h"
#include "cnp/stencil/CnpStencilFactory.h"

int main(int argc, char** argv) {
    const auto stencil_factory = CnpStencilFactory();
    const auto interpreter = CnpInterpreter(stencil_factory.create());
    const auto bytecode = Bytecode(print_42, std::size(print_42));
    interpreter.execute(bytecode);

    return 0;
}
