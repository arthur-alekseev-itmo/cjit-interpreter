#pragma once
#include "RtClassHierarchy.h"

class Runtime {
public:
    static RtClassHierarchy* hierarchy();
    static void build(const std::vector<CjClassEntry>* classes);

private:
    static std::unique_ptr<RtClassHierarchy> hierarchy_;
};
