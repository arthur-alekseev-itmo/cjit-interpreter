
#include "ir/ir.hpp"
#include <vector>

#include "cnp/CnpInterpreter.h"
#include "cnp/stencil/CnpStencilFactory.h"

int main(int argc, char** argv) {

    const auto bc_math_example = std::vector(
        std::begin(basic_math_example),
        std::end(basic_math_example)
    );

    const auto bc_loop = std::vector(
        std::begin(infinite_loop_example),
        std::end(infinite_loop_example)
    );

    const auto bc_print_42 = std::vector(
        std::begin(print_42),
        std::end(print_42)
    );

    const auto bc_print_10_factorial = std::vector(
        std::begin(print_10_factorial),
        std::end(print_10_factorial)
    );

    const auto stencil_factory = CnpStencilFactory();
    const auto interpreter = CnpInterpreter(stencil_factory.create());
    interpreter.execute(bc_print_10_factorial);

    return 0;
}
