#pragma once
#include "../ir/ir.hpp"
#include <vector>

struct stencil;

typedef int64_t*(*jit_function)(int64_t*) __attribute__((preserve_none));

void interpret_naive(bytecode& code);

void interpret_cnp(bytecode& code, stencil* stencils);

