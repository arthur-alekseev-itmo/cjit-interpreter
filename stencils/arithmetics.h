#pragma once

#include "essential.h"

// ARITHMETICS

STENCIL_DECL(st_add) {
    POP(right);
    POP(left);
    PUSH(left + right);
    STENCIL_END
}

STENCIL_DECL(st_sub) {
    POP(right);
    POP(left);
    PUSH(left - right);
    STENCIL_END
}

STENCIL_DECL(st_mul) {
    POP(right);
    POP(left);
    PUSH(left * right);
    STENCIL_END
}

STENCIL_DECL(st_div) {
    POP(right);
    POP(left);
    PUSH(left / right);
    STENCIL_END
}

STENCIL_DECL(st_mod) {
    POP(right);
    POP(left);
    PUSH(left % right);
    STENCIL_END
}



// COMPARE

STENCIL_DECL(st_eq) {
    POP(right);
    POP(left);
    PUSH(left == right);
    STENCIL_END
}

STENCIL_DECL(st_neq) {
    POP(right);
    POP(left);
    PUSH(left != right);
    STENCIL_END
}

STENCIL_DECL(st_le) {
    POP(right);
    POP(left);
    PUSH(left <= right);
    STENCIL_END
}

STENCIL_DECL(st_lt) {
    POP(right);
    POP(left);
    PUSH(left < right);
    STENCIL_END
}

STENCIL_DECL(st_ge) {
    POP(right);
    POP(left);
    PUSH(left >= right);
    STENCIL_END
}

STENCIL_DECL(st_gt) {
    POP(right);
    POP(left);
    PUSH(left > right);
    STENCIL_END
}

// LOGIC

STENCIL_DECL(st_and) {
    POP(right);
    POP(left);
    PUSH(left && right);
    STENCIL_END
}

STENCIL_DECL(st_or) {
    POP(right);
    POP(left);
    PUSH(left || right);
    STENCIL_END
}

// UNARY

STENCIL_DECL(st_neg) {
    POP(arg);
    PUSH(-arg);
    STENCIL_END
}

STENCIL_DECL(st_not) {
    POP(arg);
    PUSH(!arg);
    STENCIL_END
}


