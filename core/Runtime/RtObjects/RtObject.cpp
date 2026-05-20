#include "RtObject.h"

#include "RtClass.h"
#include "../RtWrappers/RtClosureWrapper.h"

RtObject* RtObject::get_member(uint32_t field_name) {
    const auto member = class_->get_member(field_name);
    switch (member->type) {
    case MemberType::FIELD: return &data_[member->offset];
    case MemberType::CONSTRUCTOR: throw std::runtime_error("TODO: Constructor wrapping");
    case MemberType::METHOD: return new RtClosureWrapper(member->offset);
    default: throw std::runtime_error("Unreachable");
    }
}

ObjectKind RtObject::get_kind() {
    return KIND_OBJECT;
}
