#pragma once
#include "RtPrimitiveWrapper.h"
#include "../../RtObjects/RtObject.h"

class RtIntWrapper final : public RtPrimitiveWrapper {
public:
    explicit RtIntWrapper(const uint64_t value) : value_(value) {}
    RtObject* get_member(uint32_t field_name) override;
    uint64_t* get_wrapped_value_addr() override;

private:
    uint64_t value_;
};
