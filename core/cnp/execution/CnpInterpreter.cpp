#include "CnpInterpreter.h"

#include <iostream>

#include "CnpFunction.h"
#include "loguru.hpp"
#include "../codegen/CnpCodegen.h"
#include "../debug/CnpExecutionTrace.h"


CnpInterpreter* CnpInterpreter::set_instrument(bool instrument) {
    this->instrument = instrument;
    return this;
}

void CnpInterpreter::execute(const Bytecode& bytecode) const {
    std::uint64_t stack[0x1000] = {};
    std::uint64_t locals[0x1000] = {};

    auto* stack_top = stack;
    auto* locals_top = locals;

    LOG_F(1, "stack pointer: %p\n", reinterpret_cast<void*>(stack_top));
    LOG_F(1, "local pointer: %p\n", reinterpret_cast<void*>(locals_top));

    // Copy and paste makes me sad but CPP makes me sadder
    if (instrument) {
        // Inits global variable :sob:
        const auto _ = CnpExecutionTrace(stack_top, locals_top);
        const auto instrumented_bc = CnpExecutionTrace::instrument(bytecode);
        const auto fn = CnpCodegen::compile(*instrumented_bc, stencils.get());
        VLOG_S(1) << fn << std::endl;
        fn.call(stack_top, locals_top);
    } else {
        const auto fn = CnpCodegen::compile(bytecode, stencils.get());
        VLOG_S(1) << fn << std::endl;
        fn.call(stack_top, locals_top);
    }
}
