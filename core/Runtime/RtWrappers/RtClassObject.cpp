#include "RtClassObject.h"

#include "RtConstructorWrapper.h"

constexpr int OPERATOR_CALL_STRING = 1;

RtObject* RtClassObject::get_member(uint32_t field_name) {
    if (field_name == OPERATOR_CALL_STRING) {
        return RtConstructorWrapper();
    }
}
