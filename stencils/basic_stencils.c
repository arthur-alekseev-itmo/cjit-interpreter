#include <stdint.h>
#include <stdlib.h>

#define STENCIL __attribute__((preserve_none))
#define POP(name) const uint64_t name = *(--stack_top)
#define READ(name, offset) const uint64_t name = *(stack_top - (offset))
#define PUSH(value) *stack_top++ = (int64_t)(value)
#define WRITE(name, offset) *(stack_top - (offset)) = (int64_t)(value)

extern void cnp_func_hole(void) STENCIL;
extern uintptr_t cnp_value_hole_1;
extern uintptr_t cnp_value_hole_2;

#define STENCIL_HOLE_32_1(type) \
(type)((uintptr_t)&cnp_value_hole_1)

#define STENCIL_HOLE_32_2(type) \
(type)((uintptr_t)&cnp_value_hole_2)

#define STENCIL_HOLE_64_1(type) \
(type)((uintptr_t)&cnp_value_hole_1)

#define STENCIL_HOLE_64_2(type) \
(type)((uintptr_t)&cnp_value_hole_2)

#define DECLARE_STENCIL_FN(return_type, ...) \
typedef return_type(*stencil_fn)(__VA_ARGS__);

#define DECLARE_STENCIL_OUTPUT(...) \
typedef void(*stencil_output_fn)(__VA_ARGS__) STENCIL __attribute__((used)); \
stencil_output_fn stencil_output = (stencil_output_fn)&cnp_func_hole;

#define STENCIL_END \
DECLARE_STENCIL_OUTPUT(uint64_t*); \
stencil_output(stack_top);

STENCIL void st_nop(uint64_t *stack_top) {
    STENCIL_END
}

STENCIL void st_exit(uint64_t* _) {
    exit(0);
}

STENCIL void st_load_imm(uint64_t *stack_top) {
    PUSH(STENCIL_HOLE_64_1(uint64_t));
    STENCIL_END
}

STENCIL void st_add(uint64_t *stack_top) {
    POP(left);
    POP(right);
    PUSH(left + right);
    STENCIL_END
}

STENCIL void st_dup(uint64_t *stack_top) {
    POP(value);
    PUSH(value);
    PUSH(value);
    STENCIL_END
}

STENCIL void st_drop(uint64_t *stack_top) {
    POP(_);
    STENCIL_END
}

STENCIL void st_mul(uint64_t *stack_top) {
    POP(left);
    POP(right);
    PUSH(left * right);
    STENCIL_END
}

STENCIL void st_call_c_u64(uint64_t *stack_top) {
    POP(arg);
    const uint64_t fn_ptr = STENCIL_HOLE_64_1(uint64_t);
    DECLARE_STENCIL_FN(uint64_t, uint64_t);
    const stencil_fn fn = (stencil_fn)fn_ptr;
    fn(arg);
    STENCIL_END
}

STENCIL void st_call_c_stack_ptr(uint64_t *stack_top) {
    const uint64_t fn_ptr = STENCIL_HOLE_64_1(uint64_t);
    DECLARE_STENCIL_FN(uint64_t, uint64_t*);
    const stencil_fn fn = (stencil_fn)fn_ptr;
    fn(stack_top);
    STENCIL_END
}

STENCIL uint64_t* st_return(uint64_t *stack_top) {
    return stack_top;
}

STENCIL void st_jump_true(uint64_t *stack_top) {
    POP(condition);
    if (condition) {
        // 32 is used as the call is relative and will be compiled to relative jump
        void (* STENCIL fn_then)(uint64_t*) = STENCIL_HOLE_32_1(void (* STENCIL)(uint64_t*));
        return fn_then(stack_top);
    }
    void (* STENCIL fn_else)(uint64_t*) = STENCIL_HOLE_32_2(void (* STENCIL)(uint64_t*));
    return fn_else(stack_top);
}

STENCIL void st_jump(uint64_t *stack_top) {
    // 32 is used as the call is relative and will be compiled to relative jump
    void (* STENCIL jump)(uint64_t*) = STENCIL_HOLE_32_1(void (* STENCIL)(uint64_t*));
    return jump(stack_top);
    STENCIL_END
}

STENCIL void st_eq(uint64_t *stack_top) {
    POP(left);
    POP(right);
    PUSH(left == right);
    STENCIL_END
}

STENCIL void st_read_stack(uint64_t *stack_top) {
    READ(value, STENCIL_HOLE_32_1(uint32_t));
    PUSH(value);
    STENCIL_END
}

STENCIL void st_write_stack(uint64_t *stack_top) {
    POP(value);
    WRITE(value, STENCIL_HOLE_32_1(uint32_t));
    STENCIL_END
}