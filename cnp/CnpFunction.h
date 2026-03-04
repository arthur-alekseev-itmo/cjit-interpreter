#pragma once
#include <cstddef>
#include <memory>

#include "../ir/ir.hpp"
#include "stencil/CnpStencilCollection.h"

typedef size_t*(*cnp_function_ptr)(size_t*) __attribute__((preserve_none));

// TODO: Template with arguments maybe
class CnpFunction {
public:
    std::size_t* call(std::size_t* stack_top) const;
    [[nodiscard]] cnp_function_ptr get_function_ptr() const;
    CnpFunction(const CnpStencilCollection& stencils, bytecode& bc);
    ~CnpFunction();

    friend std::ostream& operator<<(std::ostream& os, const CnpFunction& fn);

private:
    std::size_t function_size_;
    cnp_function_ptr function_ptr_;
    bytecode bytecode_;
};
