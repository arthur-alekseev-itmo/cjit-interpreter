#pragma once

#include "RtWrappers/RtPrimitiveWrappers/RtPrimitiveWrapper.h"

#define BUILTIN_IMPL(name) uint64_t* name(uint64_t* stack_top, uint64_t* locals)
#define POP(name) const uint64_t name = *(--stack_top)
#define PUSH(value) *stack_top++ = (int64_t)(value)

#define BUILTIN_DEF(name) extern "C" uint64_t* name(uint64_t* stack_top, uint64_t* locals)
#define BUILTIN_ADDRESS(name) reinterpret_cast<uintptr_t>(&name)

#define TYPED_MATH_OPERATOR_WITH_UNBOX(name, op, type) BUILTIN_DEF(name) {           \
    POP(a);                                                                          \
    POP(raw_wrapper);                                                                \
    const auto wrapper = reinterpret_cast<RtPrimitiveWrapper*>(raw_wrapper);         \
    const auto b = wrapper->get_wrapped_value_addr();                                \
    const auto unwrapped = reinterpret_cast<type>(a) op *reinterpret_cast<type*>(b); \
    PUSH(new RtIntWrapper(unwrapped));                                               \
    return stack_top;                                                                \
}