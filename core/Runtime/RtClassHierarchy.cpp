//
// Created by Arthur Alekseev on 17.05.2026.
//

#include "RtClassHierarchy.h"

RtClass* RtClassHierarchy::get_class(const uint32_t name_hash) const {
    return classes_.at(name_hash).get();
}
