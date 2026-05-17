#include "Builtins.h"

#include "../runtime/RtClassHierarchy.h"
#include "../runtime/objects/RtObject.h"
#include "../runtime/objects/RtClass.h"
#include "../runtime/Runtime.h"

uint64_t* print_uint64_t(uint64_t* stack_top, uint64_t* locals) {
    const uint64_t value = *(--stack_top);
    std::cout << value << std::endl;
    return stack_top;
}

uint64_t* new_object(uint64_t* stack_top, uint64_t* locals) {
    // TODO: Size, markup, class, etc
    constexpr auto SIZE = 0x100;
    auto object_data = std::make_unique<uint8_t[]>(SIZE);
    const auto object = new RtObject(nullptr, std::move(object_data));
    *stack_top = reinterpret_cast<uint64_t>(object);
    return stack_top + 1;
}

uint64_t* get_field(uint64_t* stack_top, uint64_t* locals) {
    const auto field_name_hash = static_cast<uint32_t>(*(--stack_top));
    const auto object_addr = reinterpret_cast<RtObject*>(*(--stack_top));
    const auto content = object_addr->get_field(field_name_hash);
    *(stack_top++) = *reinterpret_cast<uint64_t*>(*content);
    return stack_top;
}

uint64_t* get_field_ref(uint64_t* stack_top, uint64_t* locals) {
    const auto field_name_hash = static_cast<uint32_t>(*(--stack_top));
    const auto object_addr = reinterpret_cast<RtObject*>(*(--stack_top));
    const auto content = object_addr->get_field(field_name_hash);
    *(stack_top++) = reinterpret_cast<uint64_t>(content);
    return stack_top + 1;
}

uint64_t* load_class(uint64_t* stack_top, uint64_t* locals) {
    const auto class_id = *(--stack_top);
    const auto cls = Runtime::hierarchy()->get_class(class_id);
    const auto cls_object = Runtime::wrap_class(k);
    return stack_top;
}
