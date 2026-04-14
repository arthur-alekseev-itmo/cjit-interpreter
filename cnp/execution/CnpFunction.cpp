#include "CnpFunction.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sys/mman.h>

#define ADVANCE(size) ip += size; break
#define JUMP_SIZE 5
#define EXTRACT_ARGUMENT_VALUE(type, offset) *reinterpret_cast<const type*>(&bc_slice[ip + (offset)])
#define EXTRACT_ARGUMENT_PTR(type, offset) reinterpret_cast<const type*>(&bc_slice[ip + (offset)])

namespace
{
    std::tuple<uint8_t*, cnp_function_ptr, std::size_t> allocate_jit_function(
        const std::size_t data_size,
        const std::size_t instructions_size
    ) {
        const auto total_size = instructions_size + data_size;

        const auto result = static_cast<uint8_t*>(mmap(
            nullptr,
            total_size,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        ));

        assert (result != MAP_FAILED);
        const auto function = reinterpret_cast<cnp_function_ptr>(result + data_size);
        return {result, function, total_size};
    }
}


std::uint64_t* CnpFunction::call(std::uint64_t* stack_top, std::uint64_t* locals) const {
    const auto rc = mprotect(
        this->memory_begin_,
        this->memory_size_,
        PROT_EXEC | PROT_READ
    );

    if (rc) {
        perror("Failed to create executable memory (m protect)");
    }

    std::cout << reinterpret_cast<void*>(function_ptr_) << std::endl;
    return this->function_ptr_(stack_top, locals);
}

CnpFunction::CnpFunction(
    const std::size_t data_size,
    const std::size_t instructions_size
) {
    const auto [
        data_pool_ptr,
        function_ptr,
        total_size
    ] = allocate_jit_function(data_size, instructions_size);

    this->memory_begin_ = data_pool_ptr;
    this->function_ptr_ = function_ptr;

    this->memory_size_ = total_size;
    this->function_size_ = instructions_size;
    this->data_size_ = data_size;
}

// TODO: Maybe free function ptr?
CnpFunction::~CnpFunction() {
    munmap(
        this->memory_begin_,
        this->function_size_
    );
}

uint8_t* CnpFunction::data_ptr() const {
    return this->memory_begin_;
}


uint8_t* CnpFunction::function_ptr() const {
    return reinterpret_cast<uint8_t*>(this->function_ptr_);
}


std::ostream& operator<<(std::ostream& os, const CnpFunction& fn) {
    std::cout << std::hex << std::setfill('0');

    std::cout << std::endl << "code: ";
    for (std::size_t i = 0; i < fn.function_size_; i++) {
        const auto byte = reinterpret_cast<uint8_t*>(fn.function_ptr_)[i];
        std::cout << std::setw(2) << static_cast<unsigned int>(byte) << " ";
    }

    std::cout << std::endl << "data: ";
    for (std::size_t i = 0; i < fn.memory_size_ - fn.function_size_; i++) {
        const auto byte = reinterpret_cast<uint8_t*>(fn.memory_begin_)[i];
        std::cout << std::setw(2) << static_cast<unsigned int>(byte) << " ";
    }

    std::cout << std::dec << std::endl; // Reset to decimal for subsequent prints
    return os;
}
