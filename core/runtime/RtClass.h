#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

class RtMethodData;

class RtClass {
public:
    // Searches within this class, then within superclasses
    uint32_t get_method_offset(uint32_t method_name);

    // Searches within this class, then within superclasses
    uint32_t get_field_offset(uint32_t field_name);

    std::string get_name();

private:
    std::string name_;
    std::shared_ptr<RtClass> super_;
    std::vector<RtClass> interfaces_;
    std::unordered_map<uint32_t, RtMethodData> methods_;
    std::unordered_map<uint32_t, uint32_t> fields_;
};


struct RtMethodData {
    uint32_t get_bytecode_offset();
    uint32_t bytecode_offset_;
    cnp_function_ptr function_;
};