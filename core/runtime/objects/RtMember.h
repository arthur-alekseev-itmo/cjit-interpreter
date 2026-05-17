#pragma once
#include <cstdint>

#include "../../bytecode/file/BytecodeClass.h"

struct RtMember {
    uint32_t name_hash;
    MemberType type;
    uint32_t offset;
};
