#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "BytecodeClass.h"
#include "Bytecode.h"

struct BytecodeHeader {
    char magic[4];
    uint32_t total_size;
    uint32_t classes_offset;
    uint32_t bytecode_offset;
    uint32_t constants_offset;
};

struct BytecodeBytecodeHeader {
    uint32_t size;
};

struct BytecodeClassesHeader {
    uint32_t class_count;
};

struct BytecodeClassHeader {
    uint32_t name;
    uint32_t super;
    uint32_t interface_count;
    uint32_t member_count;
};

struct BytecodeMemberEntry {
    uint32_t name;
    uint32_t type;
    uint32_t offset;
};

struct BytecodeConstantsHeader {
    uint32_t constants_count;
};

struct BytecodeConstantEntry {
    uint32_t size;
    uint32_t offset;
};

class BytecodeFile {
public:
    BytecodeFile(
        std::unique_ptr<Bytecode> bytecode,
        const std::vector<CjClassEntry> &classes
    ) : bytecode_(std::move(bytecode)), classes_(classes) {}

    [[nodiscard]] Bytecode* get_bytecode() const {
        return bytecode_.get();
    }

    [[nodiscard]] const std::vector<CjClassEntry>* get_classes() const {
        return &classes_;
    }

private:
    std::unique_ptr<Bytecode> bytecode_;
    std::vector<CjClassEntry> classes_;
    std::vector<CjConstant> constants_;
};

