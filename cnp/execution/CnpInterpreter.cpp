#include "CnpInterpreter.h"

#include <iostream>

#include "CnpFunction.h"
#include "../codegen/CnpCodegen.h"
#include "../debug/CnpExecutionTrace.h"

#define INSTRUMENT_DEBUG

void CnpInterpreter::execute(const Bytecode& bytecode) const {
    std::uint64_t stack[0x1000] = {};
    std::uint64_t locals[0x1000] = {};
#ifdef INSTRUMENT_DEBUG
    // Inits global variable :sob:
    const auto _ = CnpExecutionTrace(stack);
    const auto instrumented_bc = CnpExecutionTrace::instrument(bytecode);
    const auto fn = CnpCodegen::compile(*instrumented_bc, stencils.get());
#else
    const auto fn = CnpCodegen::compile(bytecode, stencils.get());
#endif
    std::uint64_t* stack_top = stack;
    std::uint64_t* locals_top = locals;
    std::cout << "stack " << reinterpret_cast<void*>(stack_top) << std::endl;
    std::cout << "local " << reinterpret_cast<void*>(locals_top) << std::endl;
    std::cout << fn;
    fn.call(stack_top, locals_top);
}
