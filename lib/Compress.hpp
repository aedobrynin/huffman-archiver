#pragma once

#include "BinaryTree.hpp"
#include "InputBitStream.hpp"
#include "InternalStructures.hpp"
#include "OutputBitStream.hpp"

namespace Archiver {
void Compress(const std::string& archive_name, const std::vector<std::string>& files_to_archive);

void CompressFile(const std::string& file_name, InputBitStream& in, OutputBitStream& out, bool is_last);

FrequencyList GetFrequencyList(const std::string& filename,
                               InputBitStream& ibitstream,
                               const std::vector<ControlCharacters>& additional_characters);

BinaryTree* GetBinaryTree(const FrequencyList& frequency_list);

Codebook GetCodebook(const BinaryTree* tree_root);
void CanonizeCodebook(Codebook& codebook);
CodebookData GetCodebookData(Codebook codebook);

EncodingTable GetEncodingTable(Codebook codebook);
void Encode(InputBitStream& in, const EncodingTable& encoding_table,
            OutputBitStream& out, ControlCharacters last_character);
}  // namespace Archiver
