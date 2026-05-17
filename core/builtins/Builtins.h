#pragma once
#include <iostream>

uint64_t* print_uint64_t(uint64_t* stack_top, uint64_t* locals);
uint64_t* new_object(uint64_t* stack_top, uint64_t* locals);
uint64_t* get_field(uint64_t* stack_top, uint64_t* locals);
uint64_t* get_field_ref(uint64_t* stack_top, uint64_t* locals);
uint64_t* load_class(uint64_t* stack_top, uint64_t* locals);

inline auto print_uint64_t_addr = reinterpret_cast<uintptr_t>(&print_uint64_t);

inline uintptr_t builtin_addresses[] = {
    print_uint64_t_addr
};