//
// Created by Arthur Alekseev on 05.07.2026.
//

#include "ClassVTable.hpp"

#include <algorithm>
#include <ranges>

namespace {
    inline size_t blaze_lower_bound(const uint32_t* arr, size_t size, const uint32_t target) {
        size_t base = 0;
        while (size > 1) {
            const auto half = size / 2;
            base = (arr[base + half] < target) ? (base + half) : base;
            size -= half;
        }
        return base + (arr[base] < target);
    }

    inline size_t blaze_linear(const uint32_t* arr, size_t size, const uint32_t target) {
        for (size_t i = 0; i < size; ++i) {
            if (arr[i] == target) return i;
        }
        return size;
    }
}

const RtMember* ClassVTable::lookup(uint32_t member) const {
    const auto position = blaze_linear(keys_.data(), keys_.size(), member);
    if (keys_[position] != member)
        return nullptr;
    return &members_[position];
}

ClassVTable::ClassVTable(std::unordered_map<uint32_t, RtMember> map) {
    for (const auto& key : map | std::views::keys) {
        keys_.push_back(key);
    }
    std::ranges::sort(keys_.begin(), keys_.end());
    for (const auto& key : keys_) {
        members_.push_back(map.at(key));
    }
}
