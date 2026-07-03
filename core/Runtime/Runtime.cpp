#include "Runtime.h"
#include <iostream>

#include "../Codegen/CnpCodegen.h"
#include "../Parsing/Bytecode/Data/BytecodeFile.h"

namespace {
    RtMember create_member(const CjMemberEntry member) {
        return RtMember(member.name, member.type, member.offset);
    }

    std::unordered_map<uint32_t, RtMember> create_members(
        const std::vector<CjMemberEntry> &entries
    ) {
        auto storage = std::unordered_map<uint32_t, RtMember>();
        for (const auto& member : entries) {
            storage[member.name] = create_member(member);
        }
        return storage;
    }

    std::shared_ptr<RtClass> add_class_to_hierarchy(
        const uint32_t name,
        const std::unordered_map<uint32_t, const CjClassEntry*> &class_table,
        std::unordered_map<uint32_t, std::shared_ptr<RtClass>> &hierarchy
    ) {
        if (hierarchy.contains(name)) {
            return hierarchy.at(name);
        }

        const auto cls = class_table.at(name);
        const auto members = create_members(cls->members);

        const auto super =
            cls->super == 0 ? nullptr
            : add_class_to_hierarchy(cls->super, class_table, hierarchy);

        const auto rt_class = std::make_shared<RtClass>(name, super, members);
        hierarchy[name] = rt_class;

        return rt_class;
    }

    std::vector<uintptr_t> build_instruction_starts(std::size_t instruction_count, const CodegenResult* codegen_result) {
        auto result = std::vector<uintptr_t>(instruction_count);
        for (std::size_t i = 0; i < instruction_count; i++) {
            result[i] = reinterpret_cast<uintptr_t>(codegen_result->get_instruction_address(i));
        }
        return result;
    }
}

RtClassHierarchy* Runtime::hierarchy() {
    return hierarchy_.get();
}

ConfiguredRuntime Runtime::build(const BytecodeFile* file, const CodegenResult* codegen_result) {
    auto class_table = std::unordered_map<uint32_t, const CjClassEntry*>();
    auto class_hierarchy = std::unordered_map<uint32_t, std::shared_ptr<RtClass>>();

    instruction_starts_ = build_instruction_starts(file->get_bytecode()->size(), codegen_result);

    for (const auto& cls : *file->get_classes()) {
        class_table[cls.name] = &cls;
    }

    for (const auto& cls : *file->get_classes()) {
        add_class_to_hierarchy(cls.name, class_table, class_hierarchy);
    }

    hierarchy_ = std::make_unique<RtClassHierarchy>(class_hierarchy);

    return {};
}

const uintptr_t Runtime::instruction_start(std::size_t index) {
    return instruction_starts_[index];
}

std::unique_ptr<RtClassHierarchy> Runtime::hierarchy_ = nullptr;
std::vector<uintptr_t> Runtime::instruction_starts_ = {};
BumpAllocator Runtime::allocator = BumpAllocator();