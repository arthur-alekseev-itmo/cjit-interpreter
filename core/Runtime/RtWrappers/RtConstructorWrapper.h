#pragma once

#include "../RtObjects/RtObject.h"
#include <cstdint>

#include "../Runtime.h"

class RtConstructorWrapper final : public RtObject {
public:
    RtConstructorWrapper(
        const uint32_t function_instruction,
        const RtClass* klass
    ) : RtObject(nullptr), function_address_(Runtime::instruction_start(function_instruction)), constructed_class_(klass) {}

    ObjectKind get_kind() override;

    const RtClass* get_constructed_class() const;
    const uint8_t* get_function() const;

private:
    const uint8_t* function_address_;
    const RtClass* constructed_class_;
};
