
#include "../RtObjects/RtObject.h"

class RtFunctionWrapper final : RtObject {
public:
    explicit RtFunctionWrapper(uint8_t* function_address) : function_address_(function_address) {}

    [[nodiscard]] uint8_t* get_function_address() const;
    ObjectKind get_kind() override;
    RtObject* get_member(uint32_t field_name) override;
private:
    uint8_t* function_address_;
};
