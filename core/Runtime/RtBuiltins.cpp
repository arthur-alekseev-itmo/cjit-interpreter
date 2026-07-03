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

BUILTIN_IMPL(print_int) {
    const uint64_t raw_value = *(--stack_top);
    const auto object = reinterpret_cast<RtIntWrapper*>(raw_value);
    std::cout << *object->get_wrapped_value_addr() << std::endl;
    return stack_top;
}

BUILTIN_IMPL(wrap_function_address) {
    POP(address);
    const auto wrapper = Runtime::allocator.construct<RtFunctionWrapper>(reinterpret_cast<uint8_t*>(address));
    PUSH(wrapper);
    // VLOG_F(1, "Wrapped function address: %llu at: %p", address, wrapper);
    return stack_top;
}

BUILTIN_IMPL(load_class) {
    POP(class_idx);
    const auto klass = Runtime::hierarchy()->get_class(class_idx);
    const auto wrapper = Runtime::allocator.construct<RtClassObject>(klass);
    PUSH(wrapper);
    // VLOG_F(1, "Loaded class: %llu at: %p", class_idx, wrapper);
    return stack_top;
}

BUILTIN_IMPL(get_field) {
    POP(field_idx);
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    const auto member = object->get_member(field_idx);
    PUSH(member);
    // VLOG_F(1, "Got field %llu of %p --> %p", field_idx, object, member);
    return stack_top;
}

BUILTIN_IMPL(get_field_ref) {
    POP(field_idx);
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    auto ref = object->get_member_ref(field_idx);
    // VLOG_F(1, "Getting ref member %llu of %p --> %p", field_idx, object, ref);
    PUSH(ref);
    return stack_top;
}

BUILTIN_IMPL(prepare_call_object) {
    POP(raw_object);
    const auto object = reinterpret_cast<RtObject*>(raw_object);
    // VLOG_F(1, "Calling object: %p", object);
    switch (object->get_kind()) {
    case KIND_OBJECT:
        throw std::runtime_error("Cannot call an object");
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
            const auto address = function->get_function_address();
            PUSH(address);
            // VLOG_F(1, "Unpacked function wrapper (%p) --> %p", function, address);
            return stack_top;
        }
    case KIND_CTOR: {
            const auto ctor = reinterpret_cast<RtConstructorWrapper*>(object);
            const auto new_instance = Runtime::allocator.construct<RtObject>(ctor->get_constructed_class());
            PUSH(new_instance);
            PUSH(ctor->get_function());
            // VLOG_F(1, "Constructor call (%p), new instance is: %p", ctor, new_instance);
            return stack_top;
        }
    }
    throw std::runtime_error("TODO");
}

BUILTIN_IMPL(unbox) {
    POP(raw_object);
    const auto object = reinterpret_cast<RtIntWrapper*>(raw_object);
    PUSH(*object->get_wrapped_value_addr());
    return stack_top;
}

BUILTIN_IMPL(box) {
    POP(wrapper_ty_id);
    POP(target);
    switch (wrapper_ty_id) {
    case RtWrapperType::IntWrapper: {
            const auto wrapper =  Runtime::allocator.construct<RtIntWrapper>(static_cast<uint64_t>(target));
            // VLOG_F(1, "Boxing an integer %llu, result is: %p", target, wrapper);
            PUSH(wrapper);
            
            return stack_top;
        }
    case RtWrapperType::BoolWrapper: {
            const auto wrapper =  Runtime::allocator.construct<RtIntWrapper>(static_cast<uint64_t>(target));
            // VLOG_F(1, "Boxing a boolean %llu, result is: %p", target, wrapper);
            PUSH(wrapper);
            return stack_top;
        }
    case RtWrapperType::FloatWrapper: throw std::runtime_error("TODO");
    default:
        // VLOG_F(1, "Unknown wrap type: %llu while boxing %llu", wrapper_ty_id, target);
        throw std::runtime_error("TODO: box");
    }
}

BUILTIN_IMPL(cast) {
    POP(class_id);
    POP(raw_target);
    const auto klass = Runtime::hierarchy()->get_class(class_id);
    const auto obj = reinterpret_cast<RtObject*>(raw_target);
    const auto result =  Runtime::allocator.construct<RtObject>(klass, obj->get_data());
    PUSH(result);
    // VLOG_F(1, "Casting %p from %d to %llu, result is: %p", reinterpret_cast<void*>(obj), obj->get_class()->get_id(), class_id, result);
    return stack_top;
}

BUILTIN_IMPL(new_object) {
    POP(class_id);
    const auto klass = Runtime::hierarchy()->get_class(class_id);
    const auto object =  Runtime::allocator.construct<RtObject>(klass);
    // VLOG_F(1, "Creating new object via NEW of type %llu, instance: %p", class_id, object);
    PUSH(object);
    return stack_top;
}

// Calls all the $clinits (hash=15)
BUILTIN_IMPL(init) {
    using FuncType = uint64_t* (__attribute__((preserve_none)) *)(uint64_t*, uint64_t*);
    for (auto &klass : Runtime::hierarchy()->get_classes()) {
        // VLOG_F(1, "Initializing class: %d", klass->get_id());
        const auto class_object = Runtime::allocator.construct<RtClassObject>(klass);
        PUSH(class_object);
        const auto clinit_method = klass->get_member(15);
        const auto func_offset = Runtime::instruction_start(clinit_method->offset);
        const auto func = reinterpret_cast<FuncType>(func_offset);
        func(stack_top, locals);
        POP(_);
    }
    // VLOG_F(1, "Finished static initialization");
    return stack_top;
}

uintptr_t RtBuiltins::get_function(uint32_t index) {
    if (index >= BUILTINS_COUNT) {
        throw std::runtime_error("Trying to access bad builtin: " + std::to_string(index));
    }
    return builtin_addresses[index];
}

