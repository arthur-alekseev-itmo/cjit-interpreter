#pragma once
#include <cstddef>
#include <memory>

#include "../stencil/CnpStencilCollection.h"

typedef uint64_t*(*cnp_function_ptr)(uint64_t*) __attribute__((preserve_none));

class CnpFunction {
public:
    uint64_t* call(uint64_t* stack_top) const;
    CnpFunction(std::size_t data_size, std::size_t instructions_size);
    ~CnpFunction();

    [[nodiscard]] uint8_t* data_ptr() const;
    [[nodiscard]] uint8_t* function_ptr() const;

    friend std::ostream& operator<<(std::ostream& os, const CnpFunction& fn);

private:
    std::uint8_t* memory_begin_;
    cnp_function_ptr function_ptr_;

    std::size_t memory_size_;
    std::size_t data_size_;
    std::size_t function_size_;
};
