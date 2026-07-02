#pragma once

#include <memory>

class BytecodeFile;

class CnpExecutionTrace {
public:
    explicit CnpExecutionTrace(uint64_t* base, uint64_t* locals_base);
    static void instrument(BytecodeFile& bc);

private:
    uint64_t* base_;
};
