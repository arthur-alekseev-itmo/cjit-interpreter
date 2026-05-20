#pragma once
#include <unordered_map>
#include <utility>

#include "RtObjects/RtClass.h"

class RtClassHierarchy {
public:
    explicit RtClassHierarchy(
        std::unordered_map<uint32_t, std::shared_ptr<RtClass>> classes
    ) : classes_(std::move(classes)) {}

    RtClass* get_class(uint32_t name_hash) const;

private:
    std::unordered_map<uint32_t, std::shared_ptr<RtClass>> classes_;
};
