#pragma once

#include <memory>

#include "CnpStencil.h"

class CnpStencilCollection {
public:
    [[nodiscard]] std::vector<CnpStencil> get_stencils();

    CnpStencil& operator[](const std::size_t index) {
        return stencils[index];
    }

    const CnpStencil& operator[](const std::size_t index) const {
        return stencils[index];
    }

    explicit CnpStencilCollection(
        std::vector<CnpStencil> stencils
    ) : stencils(std::move(stencils)) {}

private:
    std::vector<CnpStencil> stencils;
};