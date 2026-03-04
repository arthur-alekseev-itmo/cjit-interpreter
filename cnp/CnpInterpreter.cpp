#include "CnpInterpreter.h"

#include "CnpFunction.h"

void CnpInterpreter::execute(bytecode& bytecode) const
{
    const auto fn = CnpFunction(*this->stencils, bytecode);
    std::size_t stack[0x10] = {};
    std::size_t* stack_top = stack;
    std::cout << fn;
    fn.call(stack_top);
    std::cout << *stack_top;
}
