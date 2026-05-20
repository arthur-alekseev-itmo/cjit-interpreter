#include "RtBuiltins.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "Runtime.h"
#include "RtWrappers/RtClassObject.h"

#define BUILTIN_IMPL(name) uint64_t* name(uint64_t* stack_top, uint64_t* locals)
#define POP(name) const uint64_t name = *(--stack_top)
#define PUSH(value) *stack_top++ = (int64_t)(value)

BUILTIN_IMPL(print_int) {
    const uint64_t value = *(--stack_top);
    std::cout << value << std::endl;
    return stack_top;
}

BUILTIN_IMPL(load_class) {
    POP(class_idx);
    const auto klass = Runtime::hierarchy()->get_class(class_idx);
    const auto wrapper = RtClassObject(klass);
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(get_field) {
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(get_field_ref) {
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(prepare_call_object) {
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(unbox) {
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(box) {
    throw std::runtime_error("TODO");
}

uintptr_t RtBuiltins::get_function(uint32_t index) {
    if (index >= BUILTINS_COUNT) {
        throw std::runtime_error("Trying to access bad builtin: " + std::to_string(index));
    }
    return builtin_addresses[index];
}

