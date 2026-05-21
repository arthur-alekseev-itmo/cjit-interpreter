#include "RtClassObject.h"

#include "RtConstructorWrapper.h"

constexpr int OPERATOR_CALL_STRING = 1;

RtObject* RtClassObject::get_member(uint32_t field_name) {
    if (field_name == OPERATOR_CALL_STRING) {
        const auto ctor = this->representing_->get_static_member(OPERATOR_CALL_STRING);
        // TODO: Alloc with special allocator or remember the link
        return new RtConstructorWrapper(ctor->offset, representing_);
    }
    throw std::runtime_error("TODO: Static member resolving");
}
