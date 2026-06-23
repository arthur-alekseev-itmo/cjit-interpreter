#pragma once

#include "essential.h"

// Everyone here is essentially a foreign call to runtime

CALL_BUILTIN_RENAMED_1ARG(st_new)
CALL_BUILTIN_RENAMED_1ARG(st_get_field)
CALL_BUILTIN_RENAMED_1ARG(st_get_field_ref)
CALL_BUILTIN_RENAMED_1ARG(st_box)
CALL_BUILTIN_RENAMED(st_unbox)
CALL_BUILTIN_RENAMED_1ARG(st_load_class)
CALL_BUILTIN_RENAMED_1ARG(st_cast)
CALL_BUILTIN_RENAMED_1ARG(st_call_no_new)