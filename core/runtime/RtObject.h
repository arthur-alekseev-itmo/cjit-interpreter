#pragma once
#include <memory>


class RtClass;

class RtObject {
public:
    void call_method(uint32_t method);
    std::uint8_t* get_field(uint32_t field_name);

    explicit RtObject(
        RtClass* klass,
        std::unique_ptr<std::uint8_t*> data
    ) : class_(klass), data_(std::move(data)) {}

private:
    RtClass* class_;
    std::unique_ptr<std::uint8_t*> data_;
};
