#include "RtBuiltins.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "loguru.hpp"
#include "Runtime.h"
#include "RtWrappers/RtClassObject.h"
#include "RtWrappers/RtClosureWrapper.h"
#include "RtWrappers/RtConstructorWrapper.h"
#include "RtWrappers/RtFunctionWrapper.h"
#include "RtWrappers/RtPrimitiveWrappers/RtIntWrapper.h"
#include "RtWrappers/RtPrimitiveWrappers/RtWrapperTypes.h"

BUILTIN_IMPL(print_int)
{
    const uint64_t raw_value = *(--stack_top);
    const auto object = reinterpret_cast<RtIntWrapper*>(raw_value);
    std::cout << *object->get_wrapped_value_addr() << std::endl;
    VLOG_F(1, "Stack_top: %p", stack_top);
    return stack_top;
}

BUILTIN_IMPL(wrap_function_address) {
    POP(address);
    const auto wrapper = new RtFunctionWrapper(reinterpret_cast<uint8_t*>(address));
    PUSH(wrapper);
    VLOG_F(1, "Wrapped function address: %llu at: %p", address, wrapper);
    VLOG_F(1, "Stack_top: %p", stack_top);
    return stack_top;
}

BUILTIN_IMPL(load_class) {
    POP(class_idx);
    const auto klass = Runtime::hierarchy()->get_class(class_idx);
    const auto wrapper = new RtClassObject(klass);
    PUSH(wrapper);
    VLOG_F(1, "Loaded class: %llu at: %p", class_idx, wrapper);
    VLOG_F(1, "Stack_top: %p", stack_top);
    return stack_top;
}

BUILTIN_IMPL(get_field) {
    POP(field_idx);
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    const auto member = object->get_member(field_idx);
    PUSH(member);
    VLOG_F(1, "Got field %llu of %p --> %p", field_idx, object, member);
    VLOG_F(1, "Stack_top: %p", stack_top);
    return stack_top;
}

BUILTIN_IMPL(get_field_ref) {
    POP(field_idx);
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    auto ref = object->get_member_ref(field_idx);
    VLOG_F(1, "Getting ref member %llu of %p --> %p", field_idx, object, ref);
    PUSH(ref);
    VLOG_F(1, "Stack_top: %p", stack_top);
    return stack_top;
}

BUILTIN_IMPL(prepare_call_object) {
    VLOG_F(1, "Stack_top before pco: %p", stack_top);
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    VLOG_F(1, "Calling object: %p", object);
    switch (object->get_kind()) {
    case KIND_OBJECT:
        throw std::runtime_error("Cannot call an object");
    case KIND_CLOSURE: {
            const auto closure = reinterpret_cast<RtClosureWrapper*>(object);
            for (const auto arg : closure->get_arguments()) {
                PUSH(arg);
            }
            PUSH(closure->get_function());
            VLOG_F(1, "Stack_top: %p", stack_top);
            return stack_top;
        }
    case KIND_FUNCTION: {
            const auto function = reinterpret_cast<RtFunctionWrapper*>(object);
            const auto address = function->get_function_address();
            PUSH(address);
            VLOG_F(1, "Unpacked function wrapper (%p) --> %p", function, address);
            VLOG_F(1, "Stack_top: %p", stack_top);
            return stack_top;
        }
    case KIND_CTOR: {
            const auto ctor = reinterpret_cast<RtConstructorWrapper*>(object);
            const auto new_instance = new RtObject(ctor->get_constructed_class());
            PUSH(new_instance);
            PUSH(ctor->get_function());
            VLOG_F(1, "Constructor call (%p), new instance is: %p", ctor, new_instance);
            VLOG_F(1, "Stack_top: %p", stack_top);
            return stack_top;
        }
    }
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(unbox) {
    POP(raw_object);
    const auto object = reinterpret_cast<RtIntWrapper*>(raw_object);
    PUSH(*object->get_wrapped_value_addr());
    VLOG_F(1, "Stack_top: %p", stack_top);
    return stack_top;
}

BUILTIN_IMPL(box) {
    POP(wrapper_ty_id);
    POP(target);
    switch (wrapper_ty_id) {
    case RtWrapperType::IntWrapper: {
            const auto wrapper = new RtIntWrapper(static_cast<uint64_t>(target));
            VLOG_F(1, "Boxing an integer %llu, result is: %p", target, wrapper);
            PUSH(wrapper);
            VLOG_F(1, "Stack_top: %p", stack_top);
            return stack_top;
        }
    case RtWrapperType::BoolWrapper: {
            const auto wrapper = new RtIntWrapper(static_cast<uint64_t>(target));
            VLOG_F(1, "Boxing a boolean %llu, result is: %p", target, wrapper);
            PUSH(wrapper);
            VLOG_F(1, "Stack_top: %p", stack_top);
            return stack_top;
        }
    case RtWrapperType::FloatWrapper: throw std::runtime_error("TODO");
    default:
        VLOG_F(1, "Unknown wrap type: %llu while boxing %llu", wrapper_ty_id, target);
        throw std::runtime_error("TODO: box");
    }
}

BUILTIN_IMPL(cast) {
    POP(class_id);
    POP(raw_target);
    const auto klass = Runtime::hierarchy()->get_class(class_id);
    const auto obj = reinterpret_cast<RtObject*>(raw_target);
    VLOG_F(1, "Casting %p from %d to %llu", reinterpret_cast<void*>(obj), obj->get_class()->get_id(), class_id);
    obj->cast_to(klass);
    PUSH(obj);
    VLOG_F(1, "Stack_top: %p", stack_top);
    return stack_top;
}

uintptr_t RtBuiltins::get_function(uint32_t index) {
    if (index >= BUILTINS_COUNT) {
        throw std::runtime_error("Trying to access bad builtin: " + std::to_string(index));
    }
    return builtin_addresses[index];
}

