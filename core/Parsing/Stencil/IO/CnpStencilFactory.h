#pragma once
#include <memory>

#include "../Data/CnpStencilCollection.h"

class CnpStencilFactory {
public:
    [[nodiscard]] std::unique_ptr<CnpStencilCollection> create() const;
    CnpStencilFactory();

    CnpStencilFactory* set_stencil_directory(const std::string& path);
    CnpStencilFactory* set_stencil_binary(const std::string& path);
    CnpStencilFactory* set_recompile(bool recompile);

private:
    std::string stencil_path_;
    std::string stencil_binary_path_;
    bool recompile_;
};
