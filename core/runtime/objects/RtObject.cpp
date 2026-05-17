#include "RtObject.h"

#include "RtClass.h"

std::uint8_t* RtObject::get_field(uint32_t field_name) {
    const auto data_ptr = this->data_.get();
    throw std::runtime_error("TODO"); // this->class_->get_field_offset(field_name);
}
