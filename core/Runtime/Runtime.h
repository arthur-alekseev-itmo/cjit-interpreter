#pragma once
#include "RtClassHierarchy.h"
#include "../Codegen/CnpCodegen.h"
#include "Memory/BumpAllocator.hpp"

class BytecodeFile;

// Empty class used to mark that Runtime configuration is ok
// Will be consumed by interpreter to assure that Runtime configuration was not skipped
class ConfiguredRuntime { /* Empty class */ };

class Runtime {
public:
    static RtClassHierarchy* hierarchy();
    static ConfiguredRuntime build(const BytecodeFile* file, const CodegenResult* codegen_result);
    static const uintptr_t instruction_start(std::size_t index);

    static BumpAllocator allocator;

private:
    static std::unique_ptr<RtClassHierarchy> hierarchy_;
    static std::vector<uintptr_t> instruction_starts_;
};
