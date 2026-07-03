
#include "BumpAllocator.hpp"

#include <cassert>
#include <memory>
#include <sys/mman.h>

void* BumpAllocator::alloc(std::size_t size) {
    constexpr auto alignment = alignof(std::max_align_t);
    void* current_ptr = static_cast<uint8_t*>(mem_start_.mem) + offset_;
    std::size_t remaining_space = mem_start_.size - offset_;

    void* aligned_ptr =
        mem_start_.size > offset_
        ? std::align(alignment, size, current_ptr, remaining_space)
        : nullptr;

    if (!aligned_ptr) {
        allocate_next();

        current_ptr = static_cast<uint8_t*>(mem_start_.mem) + offset_;
        remaining_space = mem_start_.size - offset_;
        aligned_ptr = std::align(alignment, size, current_ptr, remaining_space);

        if (!aligned_ptr) return nullptr;
    }

    offset_ += size;
    return aligned_ptr;
}

void BumpAllocator::allocate_next() {
    block_size_ *= 2;

    const auto mem_start = static_cast<uint8_t*>(mmap(
        nullptr,
        block_size_,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    ));

    assert(mem_start != MAP_FAILED);

    mem_start_ = {
        .mem = mem_start,
        .size = block_size_
    };

    offset_ = 0;
    used_blocks_.push_back(mem_start_);
}

BumpAllocator::BumpAllocator() : offset_(0) {
    allocate_next();
}

BumpAllocator::~BumpAllocator() {
    for (const auto block : used_blocks_) {
        munmap(block.mem, block.size);
    }
    // TODO: Destroy other fields
}
