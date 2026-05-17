#pragma once

#include "essential.h"

// Everyone here is essentially a foreign call to runtime

#define CALL_BUILTIN_RENAMED(name) STENCIL_DECL(name) {                             \
    PUSH(STENCIL_HOLE_32_1(uint64_t));                                              \
    uint64_t* (* builtin)(uint64_t*) = STENCIL_HOLE_64_2(uint64_t* (*)(uint64_t*)); \
    stack_top = builtin(stack_top);                                                 \
    STENCIL_END                                                                     \
}

CALL_BUILTIN_RENAMED(st_new)
CALL_BUILTIN_RENAMED(st_get_field)
CALL_BUILTIN_RENAMED(st_get_field_ref)
CALL_BUILTIN_RENAMED(st_box)
CALL_BUILTIN_RENAMED(st_unbox)
CALL_BUILTIN_RENAMED(st_load_class)