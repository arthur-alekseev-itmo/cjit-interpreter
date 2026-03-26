
#include "Instruction.h"

Opcode Instruction::get_opcode() const {
    return static_cast<Opcode>(*bytecode_ptr_);
}

std::size_t Instruction::get_size() const {
    return OpcodeUtils::size(static_cast<Opcode>(*bytecode_ptr_));
}

Instruction InstructionList::current() const {
    return Instruction(current_ptr_);
}

Instruction InstructionList::next() {
    current_ptr_ += current().get_size();
    return current();
}

bool InstructionList::has_next() const {
    return bytecode_ptr_ - current_ptr_ > 0;
}
