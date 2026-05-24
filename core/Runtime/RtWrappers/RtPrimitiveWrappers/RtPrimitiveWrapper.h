#pragma once

#include "../../RtObjects/RtObject.h"

enum BytecodeOperatorNames : uint32_t {
    OPERATOR_PLUS = 2,
    OPERATOR_MINUS = 3,
    OPERATOR_MUL = 4,
    OPERATOR_DIV = 5,
    OPERATOR_MOD = 6,
    OPERATOR_LT = 7,
    OPERATOR_LE = 8,
    OPERATOR_EQ = 9,
    OPERATOR_GT = 10,
    OPERATOR_GE = 11,
    OPERATOR_NEQ = 12,
};

class RtPrimitiveWrapper : public RtObject {
public:
    virtual uint64_t* get_wrapped_value_addr() = 0;
};
