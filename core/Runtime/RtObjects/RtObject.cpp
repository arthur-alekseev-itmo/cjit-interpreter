#include "RtObject.h"

#include <cassert>

#include "RtClass.h"
#include "../RtWrappers/RtClosureWrapper.h"

RtObject* RtObject::get_member(uint32_t field_name) {
    const auto member = class_->get_member(field_name);
    switch (member->type) {
    case MemberType::FIELD:
        assert(data_->size() > member->offset);
        return (*data_)[member->offset];
    case MemberType::CONSTRUCTOR:
        throw std::runtime_error("TODO: Constructor wrapping");
    case MemberType::METHOD:
        return Runtime::allocator.construct<RtClosureWrapper>(Runtime::instruction_start(member->offset), this);
    default: throw std::runtime_error("Cannot get a static member");
    }
}

RtObject** RtObject::get_member_ref(uint32_t field_name) {
    const auto member = class_->get_member(field_name);
    switch (member->type) {
    case MemberType::FIELD:
        assert(data_->size() > member->offset);
        return &(*data_)[member->offset];
    case MemberType::CONSTRUCTOR:
        throw std::runtime_error("Cannot get ref of ctor");
    case MemberType::METHOD:
        throw std::runtime_error("Cannot get ref of method");
    default: throw std::runtime_error("Cannot get a static member");
    }
}

ObjectKind RtObject::get_kind() {
    return KIND_OBJECT;
}

void RtObject::cast_to(const RtClass* klass) {
    class_ = klass;
}

const RtClass* RtObject::get_class() const {
    return class_;
}

std::shared_ptr<std::vector<RtObject*, BumpAllocator<RtObject*>>> RtObject::get_data() const {
    return data_;
}
