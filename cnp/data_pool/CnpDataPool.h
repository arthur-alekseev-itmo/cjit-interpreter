#pragma once

#include <cstddef>
#include <cstdint>

#include "../../ir/ir.hpp"


class CnpDataPool {
public:
    CnpDataPool(
        uint8_t* data_section_begin,
        const std::size_t size
    ) : data_section_begin_(data_section_begin), size_(size) {}

    CnpDataPool() = default;

    template<typename T>
    T read(std::size_t offset) const;

    template<typename T>
    void write(std::size_t offset, T value);

private:
    uint8_t* data_section_begin_;
    std::size_t size_;
};

template <typename T>
T CnpDataPool::read(const std::size_t offset) const {
    return *reinterpret_cast<T*>(data_section_begin_ + offset);
}

template <typename T>
void CnpDataPool::write(const std::size_t offset, T value) {
    *reinterpret_cast<T*>(data_section_begin_ + offset) = value;
}
