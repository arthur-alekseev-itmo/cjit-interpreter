#pragma once

#include "../Parsing/Bytecode/Data/Bytecode.h"
#include "../Execution/CnpFunction.h"
#include "../Parsing/Stencil/Data/CnpStencilCollection.h"

class CodegenResult
{
public:
    CodegenResult(
        std::unique_ptr<CnpFunction> function,
        std::vector<std::size_t> &relative_instruction_addresses
    ) : relative_instruction_addresses_(std::move(relative_instruction_addresses)), function_(std::move(function)) {}

    [[nodiscard]] const CnpFunction* get_function() const;
    [[nodiscard]] const uint8_t* get_instruction_address(uint32_t instruction_index) const;

private:
    const std::vector<std::size_t> relative_instruction_addresses_;
    const std::unique_ptr<const CnpFunction> function_;
};

class CnpCodegen {
public:
    static CodegenResult compile(const Bytecode* bc, CnpStencilCollection* stencils);
};
