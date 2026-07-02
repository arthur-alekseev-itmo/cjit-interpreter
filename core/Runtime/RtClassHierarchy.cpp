//
// Created by Arthur Alekseev on 17.05.2026.
//

#include "RtClassHierarchy.h"

#include <ranges>

RtClass* RtClassHierarchy::get_class(const uint32_t name_hash) const {
    return classes_.at(name_hash).get();
}

std::vector<RtClass*> RtClassHierarchy::get_classes() const {
    auto result = std::vector<RtClass*>();
    for (const auto& value : classes_ | std::views::values) {
        result.push_back(value.get());
    }
    return result;
}
