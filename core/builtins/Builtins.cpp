#include "Builtins.h"

uint64_t* print_uint64_t(uint64_t* stack_top, uint64_t* locals) {
    const uint64_t value = *(--stack_top);
    std::cout << value << std::endl;
    return stack_top;
}
