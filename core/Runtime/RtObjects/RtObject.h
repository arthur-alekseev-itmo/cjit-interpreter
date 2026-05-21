#pragma once
#include <memory>
#include <vector>

#include "RtClass.h"

enum ObjectKind : uint8_t {
    KIND_OBJECT,
    KIND_CLOSURE,
    KIND_CTOR,
    KIND_FUNCTION,
};

class RtObject {
public:
    virtual ~RtObject() = default;
    virtual RtObject* get_member(uint32_t field_name);
    virtual ObjectKind get_kind();

    RtObject(
        const RtClass* klass,
        std::vector<RtObject> data
    ) : class_(klass), data_(std::move(data)) {}

    explicit RtObject(
        const RtClass* klass
    ) : class_(klass), data_(std::vector<RtObject>(klass->get_field_count())) {}

    RtObject() : class_(nullptr) {}

private:
    const RtClass* class_;
    std::vector<RtObject> data_;
};
