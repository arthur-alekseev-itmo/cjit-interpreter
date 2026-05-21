#include "RtBuiltins.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "Runtime.h"
#include "RtWrappers/RtClassObject.h"
#include "RtWrappers/RtClosureWrapper.h"
#include "RtWrappers/RtConstructorWrapper.h"
#include "RtWrappers/RtFunctionWrapper.h"

#define BUILTIN_IMPL(name) uint64_t* name(uint64_t* stack_top, uint64_t* locals)
#define POP(name) const uint64_t name = *(--stack_top)
#define PUSH(value) *stack_top++ = (int64_t)(value)

BUILTIN_IMPL(print_int) {
    const uint64_t value = *(--stack_top);
    std::cout << value << std::endl;
    return stack_top;
}

BUILTIN_IMPL(wrap_function_address) {
    POP(address);
    PUSH(new RtFunctionWrapper(reinterpret_cast<uint8_t*>(address)));
    return stack_top;
}

BUILTIN_IMPL(load_class) {
    POP(class_idx);
    const auto klass = Runtime::hierarchy()->get_class(class_idx);
    const auto wrapper = new RtClassObject(klass);
    PUSH(wrapper);
    return stack_top;
}

BUILTIN_IMPL(get_field) {
    POP(field_idx);
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    const auto member = object->get_member(field_idx);
    PUSH(member);
    return stack_top;
}

BUILTIN_IMPL(get_field_ref) {
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(prepare_call_object) {
    // TODO: WHY IS LOCALS BROKEN?
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    switch (object->get_kind()) {
    case KIND_OBJECT: throw std::runtime_error("Cannot call an object");
    case KIND_CLOSURE: {
            const auto closure = reinterpret_cast<RtClosureWrapper*>(object);
            for (const auto arg : closure->get_arguments()) {
                PUSH(arg);
            }
            PUSH(closure->get_function());
            return stack_top;
        }
    case KIND_FUNCTION: {
            const auto function = reinterpret_cast<RtFunctionWrapper*>(object);
            PUSH(function->get_function_address());
            return stack_top;
        }
    case KIND_CTOR: {
            const auto ctor = reinterpret_cast<RtConstructorWrapper*>(object);
            const auto new_instance = new RtObject(ctor->get_constructed_class());
            PUSH(new_instance);
            PUSH(new_instance);
            PUSH(ctor->get_function());
            return stack_top;
        }
    }
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

