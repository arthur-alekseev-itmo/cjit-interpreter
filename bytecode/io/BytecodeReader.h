#pragma once
#include <memory>
#include <fstream>

#include "../Bytecode.h"

class BytecodeReader {
public:
    static std::unique_ptr<Bytecode> read_stream(std::istream& source);
    static std::unique_ptr<Bytecode> read_file(const std::string& filename);
};
