#pragma once
#include <cstddef>

#include "../../ir/ir.hpp"
#include "../execution/CnpFunction.h"
#include "../stencil/CnpStencilCollection.h"

class CnpCodegen {
public:
    static CnpFunction compile(bytecode& bc, CnpStencilCollection* stencils);
};
