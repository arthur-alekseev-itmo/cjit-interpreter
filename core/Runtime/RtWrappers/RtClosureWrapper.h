#pragma once

#include "../RtObjects/RtObject.h"

class RtClosureWrapper : public RtObject {
public:
    explicit RtClosureWrapper(const uint64_t function_offset) : function_offset_(function_offset) {}
    void add_argument(RtObject* arg);

private:
    uint64_t function_offset_;
};
