#pragma once
#include "../stencil/CnpStencilCollection.h"

class CnpInterpreter {
public:
    explicit CnpInterpreter(std::unique_ptr<CnpStencilCollection> stencils) : stencils(std::move(stencils)) {}
    void execute(const bytecode& bytecode) const;

private:
    std::unique_ptr<CnpStencilCollection> stencils;
};
