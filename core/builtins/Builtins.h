#pragma once
#include <iostream>

void print_uint64_t(uint64_t* stack_top, uint64_t* locals);

inline auto print_uint64_t_addr = reinterpret_cast<uintptr_t>(&print_uint64_t);

inline uintptr_t builtin_addresses[] = {
    print_uint64_t_addr
};