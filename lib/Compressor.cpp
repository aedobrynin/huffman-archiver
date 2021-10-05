#include "Compressor.hpp"

using namespace Archiver;

void Archiver::Compressor::Compress(const std::vector<InputStreamData>& streams_to_archive,
                                    std::ostream& out) {
    compressor_impl_.Compress(streams_to_archive, out);
}

