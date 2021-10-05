#pragma once

#include "CompressorImpl.hpp"

namespace Archiver {
class Compressor {
public:
    void Compress(const std::vector<InputStreamData>& streams_to_archive,
                  std::ostream& out);
private:
    CompressorImpl compressor_impl_;
};
}  // namespace Archiver
