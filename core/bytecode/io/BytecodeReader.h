#pragma once
#include <memory>
#include <fstream>

#include "../file/BytecodeFile.h"

class BytecodeReader {
public:
    static std::unique_ptr<BytecodeFile> read_stream(std::istream& source);
    static std::unique_ptr<BytecodeFile> read_file(const std::string& filename);

private:
    static std::vector<CjClassEntry> read_classes(std::istream& source);
    static CjClassEntry read_class(std::istream& source);
    static std::unique_ptr<Bytecode> read_bytecode(std::istream& source);
};
