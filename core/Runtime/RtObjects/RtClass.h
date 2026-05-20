#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include "RtMember.h"

class RtClass {
public:
    RtClass(
        std::shared_ptr<RtClass> super,
        std::unordered_map<uint32_t, RtMember> members
    ) : super_(std::move(super)), members_(std::move(members)) {}

    [[nodiscard]] const RtMember* get_member(uint32_t name) const;
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] uint32_t get_field_count() const;

private:
    std::string name_;
    std::shared_ptr<RtClass> super_;
    std::vector<RtClass> interfaces_;
    std::unordered_map<uint32_t, RtMember> members_;
};
