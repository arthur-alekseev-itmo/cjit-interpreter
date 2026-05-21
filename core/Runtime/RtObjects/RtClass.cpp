#include "RtClass.h"

const RtMember* RtClass::get_member(const uint32_t name) const {
    if (this->members_.contains(name)) {
        return &this->members_.at(name);
    }

    // TODO: Search in interfaces then
    return this->super_->get_member(name);
}

// TODO: Merge with get_member
const RtMember* RtClass::get_static_member(uint32_t name) const {
    if (this->members_.contains(name))
        return &this->members_.at(name);
    return this->super_->get_member(name);
}

std::string RtClass::get_name() const {
    return this->name_;
}

uint32_t RtClass::get_field_count() const {
    // TODO:
    return 32;
}
