#pragma once
#include <cstddef>
#include <iostream>
#include <vector>
#include <memory>
#include <sys/mman.h>
#include <cassert>

#define PAGE_SIZE 4096

struct BumpAllocatorBlock {
    void* mem;
    std::size_t size;
};

struct BumpAllocatorState {
    std::size_t block_size = PAGE_SIZE * 256 * 64; // 64MB
    std::size_t offset = 0;
    BumpAllocatorBlock mem_start;
    std::vector<BumpAllocatorBlock> used_blocks;

    void allocate_next() {
        block_size *= 2;

        const auto mem_start_ptr = static_cast<uint8_t*>(mmap(
            nullptr,
            block_size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        ));

        assert(mem_start_ptr != MAP_FAILED);

        mem_start = {
            .mem = mem_start_ptr,
            .size = block_size
        };

        offset = 0;
        used_blocks.push_back(mem_start);
    }

    BumpAllocatorState() {
        allocate_next();
    }

    ~BumpAllocatorState() {
        for (const auto block : used_blocks) {
            munmap(block.mem, block.size);
        }
    }
};

template <typename T = void>
class BumpAllocator {
public:
    using value_type = T;

    BumpAllocator() = default;
    ~BumpAllocator() = default;

    template <typename U>
    BumpAllocator(const BumpAllocator<U>& other) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n) {
        if (n == 0) return nullptr;

        std::size_t size = n * sizeof(T);
        constexpr auto alignment = alignof(std::max_align_t);

        auto& state = get_state();
        void* current_ptr = static_cast<uint8_t*>(state.mem_start.mem) + state.offset;
        std::size_t remaining_space = state.mem_start.size - state.offset;

        void* aligned_ptr = state.mem_start.size > state.offset
            ? std::align(alignment, size, current_ptr, remaining_space)
            : nullptr;

        if (!aligned_ptr) {
            state.allocate_next();

            current_ptr = static_cast<uint8_t*>(state.mem_start.mem) + state.offset;
            remaining_space = state.mem_start.size - state.offset;
            aligned_ptr = std::align(alignment, size, current_ptr, remaining_space);

            if (!aligned_ptr) throw std::bad_alloc();
        }

        state.offset = (static_cast<uint8_t*>(aligned_ptr) - static_cast<uint8_t*>(state.mem_start.mem)) + size;
        return static_cast<T*>(aligned_ptr);
    }

    template <typename U, typename... Args>
    U* construct(Args&&... args) {
        BumpAllocator<U> rebound_allocator(*this);
        U* p = rebound_allocator.allocate(1);
        return ::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }

    void deallocate(T* p, std::size_t n) noexcept {}

    template<typename... Args>
    T* construct_single(Args&&... args) {
        auto p = allocate(1);
        return ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
    }

    template <typename U>
    bool operator==(const BumpAllocator<U>& other) const noexcept {
        return true;
    }

    template <typename U>
    bool operator!=(const BumpAllocator<U>& other) const noexcept {
        return false;
    }

    template <typename U> friend class BumpAllocator;

private:
    static BumpAllocatorState& get_state() {
        static BumpAllocatorState state;
        return state;
    }
};