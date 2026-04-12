#pragma once

#include "essential.h"

// ARITHMETICS

STENCIL_DECL(st_add) {
    POP(left);
    POP(right);
    PUSH(left + right);
    STENCIL_END
}

STENCIL_DECL(st_sub) {
    POP(left);
    POP(right);
    PUSH(right - left);
    STENCIL_END
}

STENCIL_DECL(st_mul) {
    POP(left);
    POP(right);
    PUSH(left * right);
    STENCIL_END
}

STENCIL_DECL(st_div) {
    POP(left);
    POP(right);
    PUSH(left / right);
    STENCIL_END
}

STENCIL_DECL(st_mod) {
    POP(left);
    POP(right);
    PUSH(left % right);
    STENCIL_END
}



// COMPARE

STENCIL_DECL(st_eq) {
    POP(left);
    POP(right);
    PUSH(left == right);
    STENCIL_END
}

STENCIL_DECL(st_neq) {
    POP(left);
    POP(right);
    PUSH(left != right);
    STENCIL_END
}

STENCIL_DECL(st_le) {
    POP(left);
    POP(right);
    PUSH(left <= right);
    STENCIL_END
}

STENCIL_DECL(st_lt) {
    POP(left);
    POP(right);
    PUSH(left < right);
    STENCIL_END
}

STENCIL_DECL(st_ge) {
    POP(left);
    POP(right);
    PUSH(left >= right);
    STENCIL_END
}

STENCIL_DECL(st_gt) {
    POP(left);
    POP(right);
    PUSH(left > right);
    STENCIL_END
}

// LOGIC

STENCIL_DECL(st_and) {
    POP(left);
    POP(right);
    PUSH(left && right);
    STENCIL_END
}

STENCIL_DECL(st_or) {
    POP(left);
    POP(right);
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


