
#include "Instruction.h"

#include <ostream>

Opcode Instruction::get_opcode() const {
    return static_cast<Opcode>(*bytecode_ptr_);
}

std::size_t Instruction::get_size() const {
    return OpcodeUtils::size(static_cast<Opcode>(*bytecode_ptr_));
}

std::ostream& operator<<(std::ostream& os, const Instruction& instruction) {
    const auto size = instruction.get_size();
    const auto opcode = instruction.get_opcode();
    if (size == 1) {
        os << OpcodeUtils::to_string(opcode);
    }
    if (size == 5) {
        const uint32_t argument = *instruction.get_argument<const uint32_t*>(1);
        os << OpcodeUtils::to_string(opcode) << std::string(" ") << std::to_string(argument);
    }
    if (size == 9) {
        const uint64_t argument = *instruction.get_argument<const uint64_t*>(1);
        os << OpcodeUtils::to_string(opcode) << std::string(" ") << std::to_string(argument);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const InstructionList& list) {
    for (const auto& instruction : list) {
        os << instruction << "\n";
    }
    return os;
}
