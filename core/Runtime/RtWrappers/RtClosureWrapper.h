#pragma once

#include "../Runtime.h"
#include "../RtObjects/RtObject.h"

class RtClosureWrapper final : public RtObject {
public:
    explicit RtClosureWrapper(
        const uintptr_t function_offset
    ) : function_offset_(function_offset) { }

    RtClosureWrapper(
        const uintptr_t function_offset,
        const RtObject* first_arg
    ) : function_offset_(function_offset), arguments_(std::vector<const RtObject*, BumpAllocator<const RtObject*>>(1, first_arg)){ }

    void add_argument(RtObject* arg);
    ObjectKind get_kind() override;
    RtObject* get_member(uint32_t field_name) override;

    [[nodiscard]] const uintptr_t get_function() const;
    [[nodiscard]] const std::vector<const RtObject*, BumpAllocator<const RtObject*>>* get_arguments() const;

private:
    const uintptr_t function_offset_;
    std::vector<const RtObject*, BumpAllocator<const RtObject*>> arguments_;
};
