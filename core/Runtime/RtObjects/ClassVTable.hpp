#pragma once
#include <unordered_map>
#include <vector>

#include "RtMember.h"

class ClassVTable {
public:
    [[nodiscard]] const RtMember* lookup(uint32_t member) const;
    explicit ClassVTable(std::unordered_map<uint32_t, RtMember> map);

private:
    std::vector<uint32_t> keys_;
    std::vector<RtMember> members_;
};
