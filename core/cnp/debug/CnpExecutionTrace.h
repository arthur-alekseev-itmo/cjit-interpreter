#pragma once

#include <memory>
#include "../../bytecode/Bytecode.h"

class Bytecode;

class CnpExecutionTrace {
public:
    explicit CnpExecutionTrace(uint64_t* base, uint64_t* locals_base);
    static std::unique_ptr<Bytecode> instrument(const Bytecode& bc);

private:
    uint64_t* base_;
};
