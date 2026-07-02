#include "RtConstructorWrapper.h"

ObjectKind RtConstructorWrapper::get_kind() {
    return KIND_CTOR;
}

const RtClass* RtConstructorWrapper::get_constructed_class() const {
    return constructed_class_;
}

const uintptr_t RtConstructorWrapper::get_function() const {
    return function_address_;
}
