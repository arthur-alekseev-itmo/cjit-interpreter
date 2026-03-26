#include "CnpInterpreter.h"

#include <iostream>

#include "CnpFunction.h"
#include "../codegen/CnpCodegen.h"
#include "../debug/CnpExecutionTrace.h"

#define INSTRUMENT_DEBUG

void CnpInterpreter::execute(const Bytecode& bytecode) const {
    std::uint64_t stack[0x10] = {};
#ifdef INSTRUMENT_DEBUG
    // Inits global variable :sob:
    const auto _ = CnpExecutionTrace(stack);
    const auto instrumented_bc = CnpExecutionTrace::instrument(bytecode);
    const auto fn = CnpCodegen::compile(*instrumented_bc, stencils.get());
#else
    const auto fn = CnpCodegen::compile(bytecode, stencils.get());
#endif
    std::uint64_t* stack_top = stack;
    std::cout << fn;
    fn.call(stack_top);
}
