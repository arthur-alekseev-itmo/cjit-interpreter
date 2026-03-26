#pragma once

#include "../../bytecode/Bytecode.h"
#include "../execution/CnpFunction.h"
#include "../stencil/CnpStencilCollection.h"

class CnpCodegen {
public:
    static CnpFunction compile(const Bytecode& bc, CnpStencilCollection* stencils);
};
