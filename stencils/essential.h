#pragma once

#include <stdint.h>
#include <stdlib.h>

#define STENCIL __attribute__((preserve_none))
#define POP(name) const uint64_t name = *(--stack_top)
#define READ(name, offset) const uint64_t name = *(stack_top - (offset))
#define PUSH(value) *stack_top++ = (int64_t)(value)
#define WRITE(name, offset) *(stack_top - (offset)) = (int64_t)(value)

extern void cnp_func_hole(void) STENCIL;

extern __attribute__((visibility("hidden"))) uint32_t cnp_value_hole_1;
extern __attribute__((visibility("hidden"))) uint32_t cnp_value_hole_2;
extern __attribute__((visibility("hidden"))) uint64_t cnp_value_hole_3;
extern __attribute__((visibility("hidden"))) uint64_t cnp_value_hole_4;

#define STENCIL_HOLE_32_1(type) \
(type)(cnp_value_hole_1)

#define STENCIL_HOLE_32_2(type) \
(type)(cnp_value_hole_2)

#define STENCIL_HOLE_64_1(type) \
(type)(cnp_value_hole_3)

#define STENCIL_HOLE_64_2(type) \
(type)(cnp_value_hole_4)

#define DECLARE_STENCIL_FN(return_type, ...) \
typedef return_type(*stencil_fn)(__VA_ARGS__);

#define DECLARE_STENCIL_OUTPUT(...) \
typedef void(*stencil_output_fn)(__VA_ARGS__) STENCIL; \
stencil_output_fn stencil_output = (stencil_output_fn)&cnp_func_hole;

#define STENCIL_END \
DECLARE_STENCIL_OUTPUT(uint64_t*, uint64_t*); \
stencil_output(stack_top, locals);

#define STENCIL_DECL(name) STENCIL __attribute__((used)) void name(uint64_t *stack_top, uint64_t *locals)