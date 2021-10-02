#pragma once

#include <array>
#include <unordered_map>
#include <vector>

#include "BinaryTree.hpp"
#include "InputBitStream.hpp"
#include "OutputBitStream.hpp"

namespace Archiver {
enum class ControlCharacters {
    FILENAME_END = 0b00000001,    // 256 in little-endian bits.
    ONE_MORE_FILE = 0b100000001,  // 257 in little-endian bits.
    ARCHIVE_END = 0b010000001,    // 258 in little-endian bits.
};

void Compress(const std::string& archive_name, const std::vector<std::string>& files_to_archive);
void CompressFile(const std::string& file_name, InputBitStream& in, OutputBitStream& out, bool is_last);

using FrequencyList = std::array<size_t, 1 << 9>;
FrequencyList GetFrequencyList(const std::string& filename,
                               InputBitStream& ibitstream,
                               const std::vector<ControlCharacters>& additional_characters);

BinaryTree* GetBinaryTree(const FrequencyList& frequency_list);

struct CodeWord {
    unsigned short character = 0;
    std::vector<bool> code;

    bool operator==(const CodeWord& other) const {
        return character == other.character && code == other.code;
    }
};

using Codebook = std::vector<CodeWord>;
Codebook GetCodebook(const BinaryTree* tree_root);

struct CanonicalCodebook {
    std::vector<size_t> word_count_by_bit_count;  // shifted by one index to the left.
    std::vector<unsigned short> characters;

    bool operator==(const CanonicalCodebook& other) const {
        return word_count_by_bit_count == other.word_count_by_bit_count
            && characters == other.characters;
    }
};
CanonicalCodebook GetCanonicalCodebook(Codebook regular_codebook);

using EncodingTable = std::unordered_map<unsigned short, std::vector<bool>>;
EncodingTable GetEncodingTable(Codebook codebook);

void Encode(InputBitStream& in, const EncodingTable& encoding_table,
            OutputBitStream& out, ControlCharacters last_character);
}  // namespace Archiver
