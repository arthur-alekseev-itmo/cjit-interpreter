#pragma once
#include <cstdint>
#include <memory>
#include <tgmath.h>

#include "../stencil/CnpStencilCollection.h"

struct StencilCacheHeader {
    char magic[4];
    uint8_t platform;
    uint32_t stencil_count;
    uint32_t size;
};

struct StencilCacheEntry {
    uint8_t opcode;
    uint32_t offset;
    uint32_t size;

    StencilCacheEntry(const uint8_t opcode, const uint32_t offset, const uint32_t size) {
        this->opcode = opcode;
        this->offset = offset;
        this->size = size;
    }
};

class StencilCache {
public:
    static std::unique_ptr<StencilCache> from_file(const std::string& filename);
    static std::unique_ptr<StencilCache> from_stream(std::istream& stream);
    static std::unique_ptr<StencilCache> from_stencils(CnpStencilCollection* stencils);
    std::unique_ptr<CnpStencilCollection> get_stencils();
    void save(std::ostream stream);
    void save(std::string filename);

private:
    explicit StencilCache();
    StencilCacheHeader header_;
    std::vector<StencilCacheEntry> mapping_;
    std::vector<CnpStencil> stencils_;
};