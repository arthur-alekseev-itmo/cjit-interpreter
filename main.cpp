

#include "cnp/interpreter.hpp"
#include "cnp/stencils.hpp"
#include "ir/ir.hpp"
#include <vector>

int main(int argc, char** argv) {
    const auto stencils = create_stencils();

    const auto bc_math_example = std::vector(
        std::begin(basic_math_example),
        std::end(basic_math_example)
    );

    const auto bc_loop = std::vector(
        std::begin(infinite_loop_example),
        std::end(infinite_loop_example)
    );

    interpret_cnp(bc_loop, stencils->data());

    return 0;
}
