#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include "RtMember.h"

class RtObject;

class RtClass {
public:
    RtClass(
        uint32_t id,
        std::shared_ptr<RtClass> super,
        std::unordered_map<uint32_t, RtMember> members
    ) : id_(id), super_(std::move(super)), members_(std::move(members)), static_fields_(/* TODO: */ 32) {}

    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] uint32_t get_id() const;

    [[nodiscard]] const RtMember* get_member(uint32_t name) const;
    [[nodiscard]] RtObject* get_static_field(uint32_t name) const;
    [[nodiscard]] RtObject** get_static_field_ref(uint32_t name);

    [[nodiscard]] uint32_t get_field_count() const;
    [[nodiscard]] uint32_t get_static_field_count() const;

private:
    std::string name_;
    uint32_t id_;
    std::shared_ptr<RtClass> super_;
    std::vector<RtClass> interfaces_;
    std::vector<RtObject*> static_fields_;
    std::unordered_map<uint32_t, RtMember> members_;
};
