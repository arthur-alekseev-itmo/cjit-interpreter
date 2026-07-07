#include "RtClass.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>

const RtMember* RtClass::get_member(const uint32_t name) const {
    const auto member = this->members_.lookup(name);
    if (member != nullptr) return member;

    // TODO: Search in interfaces then
    assert(this->super_ != nullptr);
    return this->super_->get_member(name);
}

RtObject* RtClass::get_static_field(const uint32_t name) const {
    assert(static_fields_.size() < name);
    return static_fields_[name];
}

RtObject** RtClass::get_static_field_ref(const uint32_t name) {
    assert(static_fields_.size() < name);
    return &static_fields_[name];
}

RtClass::RtClass(
    uint32_t id,
    std::shared_ptr<RtClass> super,
    std::unordered_map<uint32_t, RtMember> members
) : field_count_(std::ranges::count_if(members, [](const auto& p) { return p.second.type == MemberType::FIELD; })),
    static_field_count_(std::ranges::count_if(members, [](const auto& p) { return p.second.type == MemberType::STATIC_FIELD; })),
    id_(id),
    super_(std::move(super)),
    static_fields_(static_field_count_, nullptr),
    members_(std::move(members))
{ }

std::string RtClass::get_name() const {
    return this->name_;
}

uint32_t RtClass::get_id() const {
    return id_;
}

uint32_t RtClass::get_field_count() const {
    return field_count_;
}

uint32_t RtClass::get_static_field_count() const {
    return static_field_count_;
}