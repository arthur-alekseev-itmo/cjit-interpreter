#pragma once
#include <cstdint>

#include "MacroForBuiltins.h"
#include "RtClassHierarchy.h"

BUILTIN_DEF(print_int);
BUILTIN_DEF(prepare_call_object);
BUILTIN_DEF(get_field);
BUILTIN_DEF(get_field_ref);
BUILTIN_DEF(load_class);
BUILTIN_DEF(box);
BUILTIN_DEF(unbox);
BUILTIN_DEF(wrap_function_address);
BUILTIN_DEF(cast);
BUILTIN_DEF(new_object);
BUILTIN_DEF(init);

enum BuiltinIndex : uint8_t {
    PRINT_INT = 0,
    PREPARE_CALL_OBJECT = 1,
    GET_FIELD = 2,
    GET_FIELD_REF = 3,
    LOAD_CLASS = 4,
    BOX = 5,
    UNBOX = 6,
    WRAP_FUN_ADDRESS = 7,
    CAST = 8,
    NEW = 9,
    INIT = 10,

    BUILTINS_COUNT
};

inline static uintptr_t builtin_addresses[] = {
    BUILTIN_ADDRESS(print_int),
    BUILTIN_ADDRESS(prepare_call_object),
    BUILTIN_ADDRESS(get_field),
    BUILTIN_ADDRESS(get_field_ref),
    BUILTIN_ADDRESS(load_class),
    BUILTIN_ADDRESS(box),
    BUILTIN_ADDRESS(unbox),
    BUILTIN_ADDRESS(wrap_function_address),
    BUILTIN_ADDRESS(cast),
    BUILTIN_ADDRESS(new_object),
    BUILTIN_ADDRESS(init)
};

class RtBuiltins {
public:
    static uintptr_t get_function(uint32_t index);
};
