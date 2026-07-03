#include "RtClassObject.h"

#include "RtClosureWrapper.h"
#include "RtConstructorWrapper.h"

constexpr int OPERATOR_CALL_STRING = 1;

RtObject* RtClassObject::get_member(uint32_t field_name) {
    const auto member = this->representing_->get_member(field_name);
    switch (member->type) {
        case MemberType::CONSTRUCTOR:
            return Runtime::allocator.construct<RtConstructorWrapper>(member->offset, representing_);
        case MemberType::STATIC_FIELD:
            return representing_->get_static_field(member->offset);
        case MemberType::STATIC_METHOD:
            return Runtime::allocator.construct<RtClosureWrapper>(Runtime::instruction_start(member->offset), static_cast<RtObject*>(this));
        default: throw std::runtime_error("Error: accessing non-static method or fields from class object");
    }
}

RtObject** RtClassObject::get_member_ref(uint32_t field_name) {
    const auto member = this->representing_->get_member(field_name);
    switch (member->type) {
        case MemberType::STATIC_FIELD: return representing_->get_static_field_ref(member->offset);
        default: throw std::runtime_error("Error: accessing non-static method or fields from class object");
    }
}


