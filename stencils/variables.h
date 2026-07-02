#pragma once

#include "essential.h"

STENCIL_DECL(st_load_local) {
    const uint32_t index = STENCIL_HOLE_32_1(uint32_t);
    PUSH(locals[index]);
    STENCIL_END
}

STENCIL_DECL(st_store_local) {
    POP(value);
    const uint32_t index = STENCIL_HOLE_32_1(uint32_t);
    locals[index] = value;
    STENCIL_END
}

STENCIL_DECL(st_get_local_ref) {
    const uint32_t index = STENCIL_HOLE_32_1(uint32_t);
    PUSH(&locals[index]);
    STENCIL_END
}

STENCIL_DECL(st_get_global_ref) {
    // TODO!!
    exit(1);
    STENCIL_END
}

STENCIL_DECL(st_get_closure_ref) {
    // TODO!!
    exit(1);
    STENCIL_END
}

STENCIL_DECL(st_write_to_ref) {
    POP(value);
    POP(ref);
    *((uint64_t*)ref) = value;
    PUSH(value);
    STENCIL_END
}

STENCIL_DECL(st_allocate_locals) {
    const uint32_t count = STENCIL_HOLE_32_1(uint32_t);
    STENCIL_END
}

STENCIL_DECL(st_load_global) {
    // TODO!!
    exit(1);
    STENCIL_END
}

STENCIL_DECL(st_store_global) {
    // TODO!!
    exit(1);
    STENCIL_END
}

STENCIL_DECL(st_load_closure) {
    // TODO!!
    exit(1);
    STENCIL_END
}

STENCIL_DECL(st_store_closure) {
    // TODO!!
    exit(1);
    STENCIL_END
}


STENCIL_DECL(st_function_address) {
    // Hardcoded
    PUSH(STENCIL_HOLE_64_1(uint64_t*));
    uint64_t* (* wrap_function)(uint64_t*, uint64_t*) = STENCIL_HOLE_64_2(uint64_t* (*)(uint64_t*, uint64_t*));
    stack_top = wrap_function(stack_top, locals);
    STENCIL_END
}