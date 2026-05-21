#include "RtClosureWrapper.h"

#include <cassert>

const int OPERATOR_CALL_STRING = 1;

void RtClosureWrapper::add_argument(RtObject* arg) {
    this->arguments_.push_back(arg);
}

ObjectKind RtClosureWrapper::get_kind() {
    return KIND_CLOSURE;
}

RtObject* RtClosureWrapper::get_member(uint32_t field_name) {
    if (field_name == OPERATOR_CALL_STRING) {
        return this;
    }
    return nullptr;
}

const uint8_t* RtClosureWrapper::get_function() const {
    assert(function_offset_ != nullptr);
    return this->function_offset_;
}


const std::vector<const RtObject*> RtClosureWrapper::get_arguments() const {
    return this->arguments_;
}
