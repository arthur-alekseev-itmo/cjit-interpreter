#pragma once
#include "../Parsing/Stencil/Data/CnpStencilCollection.h"
#include "../../Parsing/BytecodeFile/Data/Bytecode.h"

class CnpInterpreter {
public:
    explicit CnpInterpreter(std::unique_ptr<CnpStencilCollection> stencils) : stencils(std::move(stencils)) {}
    CnpInterpreter* set_instrument(bool instrument);
    void execute(const Bytecode& bytecode) const;

private:
    std::unique_ptr<CnpStencilCollection> stencils;
    bool instrument = false;
};
