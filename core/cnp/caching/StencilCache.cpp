#include "StencilCache.h"

#include <cassert>
#include <fstream>

namespace {
    void check_input(std::istream &stream) {
        stream.seekg(0, std::ios::end);
        if (!stream.good()) {
            throw std::invalid_argument("Input stream is not good");
        }

        stream.seekg(0, std::ios::beg);
    }
}

std::unique_ptr<StencilCache> StencilCache::from_file(const std::string& filename) {
    std::ifstream input(filename, std::ios_base::binary);

    if (!input.is_open()) {
        throw std::invalid_argument("Can't read input stream");
    }

    return from_stream(input);
}

std::unique_ptr<StencilCache> StencilCache::from_stream(std::istream& stream) {
    check_input(stream);
    StencilCacheHeader header{};
    stream.read(
        reinterpret_cast<char*>(&header),
        sizeof(StencilCacheHeader)
    );
    assert(strcmp(header.magic, "CJSC"));

    auto buffer = std::vector<uint8_t>(header.size);
    stream.read(
        reinterpret_cast<char*>(buffer.data()),
        header.size
    );

    const auto stencil_declarations = reinterpret_cast<const StencilCacheEntry*>(buffer.data());
    const auto content_offset = header.stencil_count * sizeof(StencilCacheEntry);
    const auto stencil_content = reinterpret_cast<const uint8_t*>(buffer.data()[content_offset]);

    for (std::size_t i = 0; i < header.stencil_count; i++) {
        const auto declaration = stencil_declarations[i];

    }
}

std::unique_ptr<StencilCache> StencilCache::from_stencils(CnpStencilCollection* stencils)
{
    auto cache = StencilCache();
    cache.stencils_ = stencils->get_stencils();
    cache.header_ = StencilCacheHeader();
    cache.header_.stencil_count = cache.stencils_.size();
    cache.header_.platform = 0xFF; // TODO!!

    std::size_t offset = sizeof(StencilCacheHeader) + cache.header_.stencil_count * sizeof(StencilCacheEntry);
    std::uint8_t current_opcode = 0;
    for (const auto stencil : cache.stencils_) {
        cache.mapping_.emplace_back(current_opcode++, offset, stencil.size());
        offset += stencil.size();
    }

    return std::make_unique<StencilCache>(cache);
}

std::unique_ptr<CnpStencilCollection> StencilCache::get_stencils() {
    return std::make_unique<CnpStencilCollection>(stencils_);
}

void StencilCache::save(std::ostream stream) {
}

void StencilCache::save(std::string filename) {
}

StencilCache::StencilCache() : header_() {
    stencils_ = std::vector<CnpStencil>();
    mapping_ = std::vector<StencilCacheEntry>();
}


