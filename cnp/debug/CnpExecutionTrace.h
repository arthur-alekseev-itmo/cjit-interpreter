#pragma once
#include <cstdint>

#include "../../ir/ir.hpp"


class CnpExecutionTrace {
public:
    explicit CnpExecutionTrace(uint64_t* base);
    static bytecode instrument(bytecode& bc);

private:
    uint64_t* base_;
};
