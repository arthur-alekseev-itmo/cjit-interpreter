#pragma once

#include "../Runtime.h"
#include "../RtObjects/RtObject.h"

class RtClosureWrapper final : public RtObject {
public:
    explicit RtClosureWrapper(
        const uint32_t function_offset
    ) : function_offset_(Runtime::instruction_start(function_offset)) { }

    void add_argument(RtObject* arg);
    ObjectKind get_kind() override;
    RtObject* get_member(uint32_t field_name) override;

    [[nodiscard]] const uint8_t* get_function() const;
    [[nodiscard]] const std::vector<const RtObject*> get_arguments() const;

private:
    const uint8_t* function_offset_;
    std::vector<const RtObject*> arguments_;
};
