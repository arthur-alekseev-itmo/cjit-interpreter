//
// Created by Arthur Alekseev on 17.05.2026.
//

#include "Runtime.h"

#include <iostream>

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
        const auto rt_class = std::make_shared<RtClass>(super, members);
        hierarchy[name] = rt_class;

        return rt_class;
    }
}

RtClassHierarchy* Runtime::hierarchy() {
    return hierarchy_.get();
}

void Runtime::build(const std::vector<CjClassEntry>* classes) {
    auto class_table = std::unordered_map<uint32_t, const CjClassEntry*>();
    auto class_hierarchy = std::unordered_map<uint32_t, std::shared_ptr<RtClass>>();

    for (const auto& cls : *classes) {
        class_table[cls.name] = &cls;
    }

    for (const auto& cls : *classes) {
        add_class_to_hierarchy(cls.name, class_table, class_hierarchy);
    }

    hierarchy_ = std::make_unique<RtClassHierarchy>(class_hierarchy);
}

std::unique_ptr<RtClassHierarchy> Runtime::hierarchy_ = nullptr;
