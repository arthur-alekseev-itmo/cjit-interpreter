#include "RtClass.h"

const RtMember* RtClass::get_member(const uint32_t name) const {
    if (this->members_.contains(name)) {
        return &this->members_.at(name);
    }

    // TODO: Search in interfaces then
    return this->super_->get_member(name);
}

RtObject* RtClass::get_static_field(uint32_t name) const {
    return static_fields_[name];
}

RtObject** RtClass::get_static_field_ref(uint32_t name)  {
    return &static_fields_[name];
}

std::string RtClass::get_name() const {
    return this->name_;
}

uint32_t RtClass::get_id() const {
    return id_;
}

uint32_t RtClass::get_field_count() const {
    // TODO:
    return 32;
}

uint32_t RtClass::get_static_field_count() const {
    // TODO:
    return 32;
}
