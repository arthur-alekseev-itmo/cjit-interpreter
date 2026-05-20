#pragma once
#include "../RtObjects/RtObject.h"

class RtClassObject : RtObject {
public:
    explicit RtClassObject(RtClass* klass) : RtObject(klass) {}

    RtObject* get_member(uint32_t field_name) override;
};
