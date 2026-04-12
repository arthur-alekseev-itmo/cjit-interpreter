#pragma once

#include "essential.h"

STENCIL_DECL(st_call_c_v_u64) {
    POP(arg);
    const uint64_t fn_ptr = STENCIL_HOLE_64_1(uint64_t);
    DECLARE_STENCIL_FN(uint64_t, uint64_t);
    const stencil_fn fn = (stencil_fn)fn_ptr;
    fn(arg);
    STENCIL_END
}

STENCIL_DECL(st_call_c_v_stack_ptr) {
    const uint64_t fn_ptr = STENCIL_HOLE_64_1(uint64_t);
    DECLARE_STENCIL_FN(uint64_t, uint64_t*);
    const stencil_fn fn = (stencil_fn)fn_ptr;
    fn(stack_top);
    STENCIL_END
}

STENCIL_DECL(st_call_builtin) {
    // Builtins are functions that take args from stack themselves, no need to check arity
    // Resolved in codegen, address of builtin is placed in data section, then this address is set to this hole
    void (* builtin)(uint64_t*) = STENCIL_HOLE_64_1(void (*)(uint64_t*));
    builtin(stack_top);
    STENCIL_END
}