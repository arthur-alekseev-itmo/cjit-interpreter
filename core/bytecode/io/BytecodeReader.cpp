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

std::unique_ptr<Bytecode> BytecodeReader::read_stream(std::istream& source) {
    check_input(source);

    BytecodeHeader header{};
    source.read(
        reinterpret_cast<char*>(&header),
        sizeof(BytecodeHeader)
    );

    assert(strcmp(header.magic, "CJBC"));

    auto buffer = std::vector<uint8_t>(header.size);
    source.read(
        reinterpret_cast<char*>(buffer.data()),
        header.size
    );

    return std::make_unique<Bytecode>(buffer.data(), buffer.size());
}

std::unique_ptr<Bytecode> BytecodeReader::read_file(const std::string& filename) {
    std::ifstream input(filename, std::ios_base::binary);

    if (!input.is_open()) {
        throw std::invalid_argument("Can't read input stream");
    }

    return read_stream(input);
}
