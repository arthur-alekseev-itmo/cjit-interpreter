#include "RtClass.h"

RtMember RtClass::get_member(const uint32_t field_name) {
    if (this->members_.contains(field_name)) {
        return this->members_[field_name];
    }

    // TODO: Search in interfaces then
    return this->super_->get_member(field_name);
}

std::string RtClass::get_name() {
    return this->name_;
}
