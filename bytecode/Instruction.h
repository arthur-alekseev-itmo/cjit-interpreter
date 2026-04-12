#pragma once
#include "Bytecode.h"
#include "Opcode.h"


class Instruction {
public:
    [[nodiscard]] Opcode get_opcode() const;
    [[nodiscard]] std::size_t get_size() const;
    template<typename T> T get_argument(std::size_t offset) const;

    explicit Instruction(const uint8_t* bytecode_ptr) : bytecode_ptr_(bytecode_ptr) {}

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instruction);

private:
    const uint8_t* bytecode_ptr_;
};

template <typename T>
T Instruction::get_argument(const std::size_t offset) const {
    return reinterpret_cast<T>(bytecode_ptr_ + offset);
}


class InstructionList {
public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Instruction;
        using difference_type = std::ptrdiff_t;

        explicit Iterator(
            const uint8_t* ptr = nullptr,
            const uint8_t* end = nullptr
        ) : ptr_(ptr), end_(end) {
            if (ptr_ && ptr_ >= end_) ptr_ = nullptr;
        }

        Instruction operator*() const { return Instruction(ptr_); }

        Iterator& operator++() {
            if (!ptr_) {
                return *this;
            }

            ptr_ += Instruction(ptr_).get_size();
            if (ptr_ >= end_) ptr_ = nullptr;
            return *this;
        }

        Iterator operator++(int) {
            const auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        const uint8_t* ptr_;
        const uint8_t* end_;
    };

    [[nodiscard]] Iterator begin() const { return Iterator(bytecode_ptr_, bytecode_ptr_ + bytecode_size_); }
    [[nodiscard]] Iterator end() const { return Iterator(); }

    explicit InstructionList(const Bytecode& bytecode)
        : bytecode_ptr_(bytecode.data()), current_ptr_(bytecode.data()), bytecode_size_(bytecode.size()) {}

    explicit InstructionList(const Bytecode* bytecode)
        : InstructionList(*bytecode) {}

    friend std::ostream& operator<<(std::ostream& os, const InstructionList& list);

private:
    const uint8_t* bytecode_ptr_;
    const uint8_t* current_ptr_;
    std::size_t bytecode_size_;
};
