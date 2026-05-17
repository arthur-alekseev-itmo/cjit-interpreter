#include "BytecodeReader.h"

#include <cassert>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include "../Bytecode.h"

namespace {
    void check_input(std::istream &stream) {
        stream.seekg(0, std::ios::end);
        if (!stream.good()) {
            throw std::invalid_argument("Input stream is not good");
        }

        stream.seekg(0, std::ios::beg);
    }
}

std::vector<CjClassEntry> BytecodeReader::read_classes(std::istream& source) {
    BytecodeClassesHeader header{};
    source.read(
        reinterpret_cast<char*>(&header),
        sizeof(BytecodeClassesHeader)
    );

    auto classes = std::vector<CjClassEntry>();

    for (auto i = 0; i < header.class_count; i++) {
        classes.push_back(read_class(source));
    }

    return classes;
}

CjClassEntry BytecodeReader::read_class(std::istream& source) {
    BytecodeClassHeader class_header{};
    source.read(
        reinterpret_cast<char*>(&class_header),
        sizeof(BytecodeClassHeader)
    );

    // TODO: remove assert and make proper interfaces
    assert(class_header.interface_count == 0);

    auto entry = CjClassEntry(class_header.name, class_header.super);

    for (int i = 0; i < class_header.member_count; i++) {
        BytecodeMemberEntry member_header{};
        source.read(
            reinterpret_cast<char*>(&member_header),
            sizeof(BytecodeMemberEntry)
        );

        auto member = CjMemberEntry(
            member_header.name,
            static_cast<MemberType>(member_header.type),
            member_header.offset
        );

        entry.members.push_back(member);
    }

    return entry;
}

std::unique_ptr<Bytecode> BytecodeReader::read_bytecode(std::istream& source) {
    BytecodeBytecodeHeader header{};
    source.read(
        reinterpret_cast<char*>(&header),
        sizeof(BytecodeBytecodeHeader)
    );

    auto buffer = std::vector<uint8_t>(header.size);
    source.read(
        reinterpret_cast<char*>(buffer.data()),
        header.size
    );

    return std::make_unique<Bytecode>(buffer.data(), buffer.size());
}

std::unique_ptr<BytecodeFile> BytecodeReader::read_stream(std::istream& source) {
    check_input(source);

    BytecodeHeader header{};
    source.read(
        reinterpret_cast<char*>(&header),
        sizeof(BytecodeHeader)
    );

    assert(strcmp(header.magic, "CJBF"));

    source.seekg(header.classes_offset, std::ios_base::seekdir::beg);
    const auto classes = read_classes(source);

    source.seekg(header.bytecode_offset, std::ios_base::seekdir::beg);
    auto bytecode = read_bytecode(source);

    // TODO: Constants

    return std::make_unique<BytecodeFile>(std::move(bytecode), classes);
}

std::unique_ptr<BytecodeFile> BytecodeReader::read_file(const std::string& filename) {
    std::ifstream input(filename, std::ios_base::binary);

    if (!input.is_open()) {
        throw std::invalid_argument("Can't read input stream");
    }

    return read_stream(input);
}
