#pragma once

#include "essential.h"

STENCIL_DECL(st_load_imm) {
    PUSH(STENCIL_HOLE_64_1(uint64_t));
    STENCIL_END
}

STENCIL_DECL(st_dup) {
    POP(value);
    PUSH(value);
    PUSH(value);
    STENCIL_END
}

STENCIL_DECL(st_drop) {
    POP(_);
    STENCIL_END
}

STENCIL_DECL(st_swap) {
    POP(first);
    POP(second);
    PUSH(first);
    PUSH(second);
    STENCIL_END
}

STENCIL_DECL(st_read_stack) {
    READ(value, STENCIL_HOLE_32_1(uint32_t));
    PUSH(value);
    STENCIL_END
}

STENCIL_DECL(st_write_stack) {
    POP(value);
    WRITE(value, STENCIL_HOLE_32_1(uint32_t));
    STENCIL_END
}
