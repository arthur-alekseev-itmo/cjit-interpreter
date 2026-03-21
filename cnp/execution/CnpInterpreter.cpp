#include "CnpInterpreter.h"

#include "CnpFunction.h"
#include "../codegen/CnpCodegen.h"
#include "../debug/CnpExecutionTrace.h"

#define INSTRUMENT_DEBUG

void CnpInterpreter::execute(bytecode& bytecode) const {
    std::size_t stack[0x10] = {};
#ifdef INSTRUMENT_DEBUG
    // Inits global variable :sob:
    const auto _ = CnpExecutionTrace(stack);
    const auto instrumented_bc = CnpExecutionTrace::instrument(bytecode);
    const auto fn = CnpCodegen::compile(instrumented_bc, stencils.get());
#else
    const auto fn = CnpCodegen::compile(bytecode, stencils.get());
#endif
    std::size_t* stack_top = stack;
    std::cout << fn;
    fn.call(stack_top);
    std::cout << *stack_top;
}
