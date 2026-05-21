#pragma once
#include <cstdint>

#include "../../Parsing/Bytecode/Data/BytecodeClass.h"

enum class MemberType : uint32_t;

struct RtMember {
    uint32_t name_hash;
    MemberType type;
    uint32_t offset;
};
