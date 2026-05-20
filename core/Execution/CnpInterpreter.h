#pragma once
#include "../Codegen/CnpCodegen.h"
#include "../Parsing/Stencil/Data/CnpStencilCollection.h"
#include "../Runtime/Runtime.h"

class CnpInterpreter {
public:
    static void execute(const CodegenResult& bytecode, ConfiguredRuntime _);

private:
    std::unique_ptr<CnpStencilCollection> stencils;
};
