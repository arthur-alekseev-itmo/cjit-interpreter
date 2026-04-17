#pragma once

#include "essential.h"

#define BASIC_MATH_UOPERATOR(name, operator) \
STENCIL_DECL(name) { \
    POP(operand); \
    PUSH(operator operand); \
    STENCIL_END \
}

#define BASIC_MATH_BOPERATOR(name, operator) \
STENCIL_DECL(name) { \
    POP(right); \
    POP(left); \
    PUSH(left operator right); \
    STENCIL_END \
}

// ARITHMETICS
BASIC_MATH_BOPERATOR(st_add, +)
BASIC_MATH_BOPERATOR(st_sub, -)
BASIC_MATH_BOPERATOR(st_mul, *)
BASIC_MATH_BOPERATOR(st_div, /)
BASIC_MATH_BOPERATOR(st_mod, %)

// COMPARE
BASIC_MATH_BOPERATOR(st_eq, ==)
BASIC_MATH_BOPERATOR(st_neq, !=)
BASIC_MATH_BOPERATOR(st_le, <=)
BASIC_MATH_BOPERATOR(st_lt, <)
BASIC_MATH_BOPERATOR(st_ge, >=)
BASIC_MATH_BOPERATOR(st_gt, >)

// BITWISE
BASIC_MATH_BOPERATOR(st_bit_and, &)
BASIC_MATH_BOPERATOR(st_bit_or, |)
BASIC_MATH_BOPERATOR(st_bit_xor, ^)

// LOGIC
BASIC_MATH_BOPERATOR(st_and, &&)
BASIC_MATH_BOPERATOR(st_or, ||)

// UNARY
BASIC_MATH_UOPERATOR(st_neg, -)
BASIC_MATH_UOPERATOR(st_not, !)

