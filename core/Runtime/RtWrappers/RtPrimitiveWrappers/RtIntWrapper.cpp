#include "RtIntWrapper.h"

#include "loguru.hpp"
#include "../RtClosureWrapper.h"
#include "../../MacroForBuiltins.h"

TYPED_MATH_OPERATOR_WITH_UNBOX(int_add, +, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_sub, -, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_mul, *, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_div, /, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_mod, %, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_lt, <, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_le, <=, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_eq, ==, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_neq, !=, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_gt, >, uint64_t)
TYPED_MATH_OPERATOR_WITH_UNBOX(int_ge, >=, uint64_t)
TYPED_MATH_UNARY_OPERATOR_WITH_UNBOX(int_ng, -, uint64_t)
TYPED_MATH_UNARY_OPERATOR_WITH_UNBOX(int_not, !, uint64_t)

#define RETURN_APPLIED_FUNCTION(function) return new RtClosureWrapper(BUILTIN_ADDRESS(function), reinterpret_cast<RtObject*>(this->value_))


RtObject* RtIntWrapper::get_member(uint32_t field_name) {
    switch (field_name) {
    case OPERATOR_PLUS: RETURN_APPLIED_FUNCTION(int_add);
    case OPERATOR_MINUS: RETURN_APPLIED_FUNCTION(int_sub);
    case OPERATOR_MUL: RETURN_APPLIED_FUNCTION(int_mul);
    case OPERATOR_DIV: RETURN_APPLIED_FUNCTION(int_div);
    case OPERATOR_MOD: RETURN_APPLIED_FUNCTION(int_mod);
    case OPERATOR_LT: RETURN_APPLIED_FUNCTION(int_lt);
    case OPERATOR_LE: RETURN_APPLIED_FUNCTION(int_le);
    case OPERATOR_EQ: RETURN_APPLIED_FUNCTION(int_eq);
    case OPERATOR_GT : RETURN_APPLIED_FUNCTION(int_gt);
    case OPERATOR_GE : RETURN_APPLIED_FUNCTION(int_ge);
    case OPERATOR_NEQ : RETURN_APPLIED_FUNCTION(int_neq);
    case OPERATOR_UNARY_NEGATE : RETURN_APPLIED_FUNCTION(int_ng);
    case OPERATOR_UNARY_NOT : RETURN_APPLIED_FUNCTION(int_not);
    default: throw std::runtime_error("Unknwon function call on int wrapper");
    }
}

uint64_t* RtIntWrapper::get_wrapped_value_addr() {
    return &this->value_;
}
