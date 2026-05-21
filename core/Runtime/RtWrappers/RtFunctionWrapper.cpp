#include "RtFunctionWrapper.h"

const uint32_t OPERATOR_CALL_STRING = 1;

uint8_t* RtFunctionWrapper::get_function_address() const {
    return function_address_;
}

RtObject* RtFunctionWrapper::get_member(uint32_t field_name) {
    if (field_name == OPERATOR_CALL_STRING) {
        return this;
    }
    return nullptr;
}

ObjectKind RtFunctionWrapper::get_kind() {
    return KIND_FUNCTION;
}
