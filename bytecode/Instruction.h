#pragma once
#include "Bytecode.h"
#include "Opcode.h"


class Instruction {
public:
    [[nodiscard]] Opcode get_opcode() const;
    [[nodiscard]] std::size_t get_size() const;
    template<typename T> T get_argument(std::size_t offset);

    explicit Instruction(const uint8_t* bytecode_ptr) : bytecode_ptr_(bytecode_ptr) {}

private:
    const uint8_t* bytecode_ptr_;
};

template <typename T>
T Instruction::get_argument(const std::size_t offset) {
    return reinterpret_cast<T>(bytecode_ptr_ + offset);
}


class InstructionList {
public:
    [[nodiscard]] Instruction current() const;
    Instruction next();
    [[nodiscard]] bool has_next() const;

    explicit InstructionList(
        const Bytecode& bytecode
    ) : bytecode_ptr_(bytecode.data()), current_ptr_(bytecode.data()), bytecode_size_(bytecode.size()) {}

private:
    const uint8_t* bytecode_ptr_;
    const uint8_t* current_ptr_;
    std::size_t bytecode_size_;

};
