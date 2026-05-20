#pragma once

#include "essential.h"

#define TODO_GOOD_LOCAL_COUNT 32

STENCIL_DECL(st_nop) {
    STENCIL_END
}

STENCIL_DECL(st_exit) {
    exit(0);
}

STENCIL_DECL(st_return) {
    return stack_top;
}

STENCIL_DECL(st_jump_true) {
    POP(condition);
    if (condition) {
        // 32 is used as the call is relative and will be compiled to relative jump
        uint64_t* (* STENCIL fn_then)(uint64_t*, uint64_t*) = STENCIL_HOLE_64_1(uint64_t* (* STENCIL)(uint64_t*, uint64_t*));
        return fn_then(stack_top, locals);
    }
    STENCIL_END
}

STENCIL_DECL(st_jump_false) {
    POP(condition);
    if (1 ^ condition) {
        // 32 is used as the call is relative and will be compiled to relative jump
        uint64_t* (* STENCIL fn_then)(uint64_t*, uint64_t*) = STENCIL_HOLE_64_1(uint64_t* (* STENCIL)(uint64_t*, uint64_t*));
        return fn_then(stack_top, locals);
    }
    STENCIL_END
}

STENCIL_DECL(st_jump_addr) {
    // 32 is used as the call is relative and will be compiled to relative jump
    uint64_t* (* STENCIL jump)(uint64_t*, uint64_t*) = STENCIL_HOLE_64_1(uint64_t* (* STENCIL)(uint64_t*, uint64_t*));
    return jump(stack_top, locals);
}

STENCIL_DECL(st_jump) {
    STENCIL_END
}

STENCIL_DECL(st_call) {
    uint64_t* (* STENCIL target)(uint64_t*, uint64_t*) = STENCIL_HOLE_64_1(uint64_t* (* STENCIL)(uint64_t*, uint64_t*));
    target(stack_top, locals + TODO_GOOD_LOCAL_COUNT);
    STENCIL_END
}

STENCIL_DECL(st_call_object) {
    POP(object_address);
    // The object called might be a Closure or Constructor
    // When this happens, some stuff might be prepared, for instance
    // - Closure arguments placed on stack
    // - New object allocated for ctor
    //
    // This function will be patched here and will do this kind of dispatch
    // Function returns address to be called or null (on stack)
    // If it returns null, then no need to call. This can happen if closure is called without full argument array
    uint64_t* (* STENCIL call_address)(uint64_t*, uint64_t*) = STENCIL_HOLE_64_1(uint64_t* (* STENCIL)(uint64_t*, uint64_t*));
    stack_top = call_address(stack_top, locals);

    POP(function_address);
    uint64_t* (* STENCIL target)(uint64_t*, uint64_t*) = (uint64_t* (* STENCIL)(uint64_t*, uint64_t*))function_address;

    if (target != NULL) {
        stack_top = target(stack_top, locals + TODO_GOOD_LOCAL_COUNT);
    }

    STENCIL_END
}