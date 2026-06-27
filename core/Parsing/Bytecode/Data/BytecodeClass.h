#pragma once

#include <cstdint>
#include <memory>
#include <vector>

enum class MemberType : uint32_t {
    FIELD = 0x1,
    METHOD = 0x2,
    CONSTRUCTOR = 0x3,
    STATIC_METHOD = 0x4,
    STATIC_FIELD = 0x5
};

struct CjMemberEntry {
    CjMemberEntry(
        const uint32_t name,
        const MemberType type,
        const uint32_t offset
    ) : name(name), type(type), offset(offset) {}

    uint32_t name;
    MemberType type;
    uint32_t offset;
};

struct CjClassEntry {
    CjClassEntry(
        const uint32_t name,
        const uint32_t super
    ) : name(name), super(super), members(std::vector<CjMemberEntry>()) {}

    uint32_t name;
    uint32_t super;
    std::vector<CjMemberEntry> members;
};

struct CjConstant {
    uint32_t size;
    std::unique_ptr<uint8_t*> data;
};