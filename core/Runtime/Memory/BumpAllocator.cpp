
#include "BumpAllocator.hpp"

#include <cassert>
#include <sys/mman.h>

void* BumpAllocator::alloc(std::size_t size) {
    const auto aligned = (size + 15) & ~15;

    if (aligned + offset_ >= mem_start_.size)
        allocate_next();

    const auto allocated = mem_start_.mem + offset_;
    offset_ += aligned;

    return allocated;
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
