#pragma once

#include <array>
#include <vector>

#include "InputBitStream.hpp"

namespace Archiver {
enum class ControlCharacters {
    FILENAME_END = 256,
    ONE_MORE_FILE = 257,
    ARCHIVE_END = 258
};

using FrequencyList = std::array<uint64_t, 1 << 9>;
FrequencyList GetFrequencyList(const std::string& filename,
                               InputBitStream& ibitstream,
                               const std::vector<ControlCharacters>& additional_characters);
}  // namespace Archiver
