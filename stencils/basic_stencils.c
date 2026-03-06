#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STENCIL __attribute__((preserve_none))
#define POP(name) const uint64_t name = *(--stack_top)
#define PUSH(value) *stack_top++ = (int64_t)(value)

extern void cnp_func_hole(void) STENCIL;
extern uint64_t cnp_hole_64_1;
extern uint64_t cnp_hole_64_2;

#define STENCIL_HOLE_1(type) \
(type)((uintptr_t)&cnp_hole_64_1)

#define STENCIL_HOLE_2(type) \
(type)((uintptr_t)&cnp_hole_64_2)

#define DECLARE_STENCIL_FN(return_type, ...) \
typedef return_type(*stencil_fn)(__VA_ARGS__);

#define DECLARE_STENCIL_OUTPUT(...) \
typedef void(*stencil_output_fn)(__VA_ARGS__) STENCIL; \
stencil_output_fn stencil_output = (stencil_output_fn)&cnp_func_hole;

STENCIL void st_nop(uint64_t *stack_top) {
    DECLARE_STENCIL_OUTPUT(uint64_t*);
    stencil_output(stack_top);
}

STENCIL void st_exit(uint64_t* _) {
    exit(0);
}

STENCIL void st_load_imm(uint64_t *stack_top) {
    PUSH(STENCIL_HOLE_1(int));
    DECLARE_STENCIL_OUTPUT(uint64_t*);
    stencil_output(stack_top);
}

STENCIL void st_add(uint64_t *stack_top) {
    POP(left);
    POP(right);
    PUSH(left + right);
    DECLARE_STENCIL_OUTPUT(uint64_t*);
    stencil_output(stack_top);
}

STENCIL void st_dup(uint64_t *stack_top) {
    POP(value);
    PUSH(value);
    PUSH(value);
    DECLARE_STENCIL_OUTPUT(uint64_t*);
    stencil_output(stack_top);
}

STENCIL void st_drop(uint64_t *stack_top) {
    POP(_);
    DECLARE_STENCIL_OUTPUT(uint64_t*);
    stencil_output(stack_top);
}

STENCIL void st_mul(uint64_t *stack_top) {
    POP(left);
    POP(right);
    PUSH(left * right);
    DECLARE_STENCIL_OUTPUT(uint64_t*);
    stencil_output(stack_top);
}

STENCIL void st_call_c_u64(uint64_t *stack_top) {
    POP(arg);
    const uint64_t fn_ptr_left = STENCIL_HOLE_1(uint32_t);
    const uint64_t fn_ptr_right = STENCIL_HOLE_2(uint32_t);
    DECLARE_STENCIL_FN(uint64_t, uint64_t);
    const stencil_fn fn = (stencil_fn)((fn_ptr_left << 32) + fn_ptr_right);
    fn(arg);
    DECLARE_STENCIL_OUTPUT(uint64_t*);
    stencil_output(stack_top);
}

STENCIL uint64_t* st_return(uint64_t *stack_top) {
    return stack_top;
}