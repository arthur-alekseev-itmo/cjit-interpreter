#include "CnpInterpreter.h"

#include <iostream>

#include "CnpFunction.h"
#include "loguru.hpp"
#include "../Codegen/CnpCodegen.h"
#include "../BytecodeTransform/CnpExecutionTrace.h"

void CnpInterpreter::execute(const CodegenResult& bytecode, ConfiguredRuntime _)
{
    std::uint64_t stack[0x10000] = {};
    std::uint64_t locals[0x10000] = {};

    auto* stack_top = stack;
    auto* locals_top = locals;

    LOG_F(1, "stack pointer: %p\n", reinterpret_cast<void*>(stack_top));
    LOG_F(1, "local pointer: %p\n", reinterpret_cast<void*>(locals_top));

    CnpExecutionTrace(stack_top, locals);

    bytecode.get_function()->call(stack_top, locals);
}
