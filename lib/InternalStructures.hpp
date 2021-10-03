#pragma once

#include <array>
#include <unordered_map>
#include <vector>

namespace Archiver {
enum class ControlCharacters {
    FILENAME_END = 0b00000001,    // 256 in little-endian bits.
    ONE_MORE_FILE = 0b100000001,  // 257 in little-endian bits.
    ARCHIVE_END = 0b010000001,    // 258 in little-endian bits.
};

using FrequencyList = std::array<size_t, 1 << 9>;

struct CodeWord {
    unsigned short character = 0;
    std::vector<bool> code;

    bool operator==(const CodeWord& other) const {
        return character == other.character && code == other.code;
    }
};

using Codebook = std::vector<CodeWord>;

struct CanonicalCodebook {
    std::vector<size_t> word_count_by_bit_count;  // shifted by one index to the left.
    std::vector<unsigned short> characters;

    bool operator==(const CanonicalCodebook& other) const {
        return word_count_by_bit_count == other.word_count_by_bit_count
            && characters == other.characters;
    }
};

using EncodingTable = std::unordered_map<unsigned short, std::vector<bool>>;
}  // namespace Archiver
