#pragma once
#include <cstddef>
#include <vector>

#define PAGE_SIZE 4096

struct BumpAllocatorBlock {
    void* mem;
    std::size_t size;
};

class BumpAllocator {
public:
    void* alloc(std::size_t size);
    template<class T, typename... Args>
    T* construct(Args... args);

    BumpAllocator();
    ~BumpAllocator();

private:
    void allocate_next();

    std::size_t block_size_ = PAGE_SIZE * 256; // 1MB
    std::size_t offset_;
    BumpAllocatorBlock mem_start_;
    std::vector<BumpAllocatorBlock> used_blocks_;
};

template <class T, typename ... Args>
T* BumpAllocator::construct(Args... args) {
    auto p = alloc(sizeof(T));
    return ::new (p) T(std::forward<Args>(args)...);
}
