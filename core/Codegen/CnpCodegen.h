#pragma once

#include "../Parsing/Bytecode/Data/Bytecode.h"
#include "../Execution/CnpFunction.h"
#include "../Parsing/Stencil/Data/CnpStencilCollection.h"

class CodegenResult {
public:
    uint8_t* get_code();
    uint64_t get_instruction_address(uint32_t instruction_index);

private:
    std::vector<uint64_t> instruction_address_;
    std::vector<uint8_t> compiled_;
};

class CnpCodegen {
public:
    static CnpFunction compile(const Bytecode& bc, CnpStencilCollection* stencils);
};
