#include "CompressorImpl.hpp"

#include <memory>
#include <queue>
#include <sstream>
#include <tuple>
#include <vector>

#include "Exceptions.hpp"

using namespace Archiver;

void Archiver::CompressorImpl::Compress(const std::vector<InputStreamData>& streams_to_archive, std::ostream& out) {
    OutputBitStream obitstream(out);
    for (size_t i = 0; i < streams_to_archive.size(); ++i) {
        InputBitStream ibitstream(streams_to_archive[i].stream);
        CompressFile(streams_to_archive[i].name, ibitstream, obitstream, i + 1 == streams_to_archive.size());
    }
}

void Archiver::CompressorImpl::CompressFile(const std::string& stream_name,
                                            InputBitStream& in,
                                            OutputBitStream& out,
                                            bool is_last) {
    std::vector<ControlCharacters> additional_characters{
        ControlCharacters::FILENAME_END,
        (is_last ? ControlCharacters::ARCHIVE_END : ControlCharacters::ONE_MORE_FILE),
    };
    auto frequency_list = GetFrequencyList(stream_name, in, additional_characters);

    std::unique_ptr<BinaryTree> binary_tree{GetBinaryTree(frequency_list)};
    auto codebook = GetCodebook(binary_tree.get());
    CanonizeCodebook(codebook);
    auto codebook_data = GetCodebookData(codebook);
    auto encoding_table = GetEncodingTable(std::move(codebook));

    unsigned short character_count = static_cast<unsigned short>(encoding_table.size());
    out.WriteBits(character_count, 9);

    for (auto character : codebook_data.characters) {
        out.WriteBits(character, 9);
    }

    for (auto word_count : codebook_data.word_count_by_bit_count) {
        out.WriteBits(static_cast<unsigned short>(word_count), 9);
    }

    std::stringstream sstream_stream_name(stream_name);
    InputBitStream ibitstream_stream_name(sstream_stream_name);
    Encode(ibitstream_stream_name, encoding_table, out, ControlCharacters::FILENAME_END);

    in.Reset();
    Encode(in, encoding_table, out, (is_last ? ControlCharacters::ARCHIVE_END : ControlCharacters::ONE_MORE_FILE));
}

FrequencyList Archiver::CompressorImpl::GetFrequencyList(const std::string& stream_name, InputBitStream& ibitstream,
                                                         const std::vector<ControlCharacters>& additional_characters) {
    FrequencyList frequency_list;
    frequency_list.fill(0);

    std::stringstream stream_name_sstream(stream_name);
    InputBitStream stream_name_ibitstream(stream_name_sstream);
    while (stream_name_ibitstream.Good()) {
        auto character = stream_name_ibitstream.ReadBits(8);
        character <<= 1;
        ++frequency_list[character];
    }

    while (ibitstream.Good()) {
        auto character = ibitstream.ReadBits(8);
        character <<= 1;
        ++frequency_list[character];
    }

    for (auto additional_character : additional_characters) {
        ++frequency_list[static_cast<unsigned short>(additional_character)];
    }

    return frequency_list;
}

BinaryTree* Archiver::CompressorImpl::GetBinaryTree(const FrequencyList& frequency_list) {
    struct CharacterData {
        size_t occurance_count;
        BinaryTree* node;
    };

    auto IsCharacterDataGreaterByOccurance = [](const CharacterData& a, const CharacterData& b) {
        return a.occurance_count > b.occurance_count;
    };

    std::priority_queue<CharacterData, std::vector<CharacterData>, decltype(IsCharacterDataGreaterByOccurance)>
        priority_queue(IsCharacterDataGreaterByOccurance);

    for (size_t i = 0; i < frequency_list.size(); ++i) {
        if (frequency_list[i] == 0) {
            continue;
        }
        priority_queue.push({.occurance_count = frequency_list[i],
                             .node = new BinaryTree(static_cast<unsigned short>(i))});
    }

    if (priority_queue.empty()) {
        return nullptr;
    }

    while (priority_queue.size() > 1) {
        auto left_son = priority_queue.top();
        priority_queue.pop();
        auto right_son = priority_queue.top();
        priority_queue.pop();

        auto parent = new BinaryTree();
        parent->SetLeftSon(left_son.node);
        parent->SetRightSon(right_son.node);
        priority_queue.push({.occurance_count = left_son.occurance_count + right_son.occurance_count,
                             .node = parent});
    }

    return priority_queue.top().node;
}

Codebook Archiver::CompressorImpl::GetCodebook(const BinaryTree* tree_root) {
    if (!tree_root) {
        return {};
    }

    Codebook codebook;
    struct WalkData {
        std::vector<bool> code;
        const BinaryTree* node;
    };
    std::queue<WalkData> queue;
    queue.push({.code = (tree_root->GetValue() == BinaryTree::NO_VALUE ? std::vector<bool>{} : std::vector<bool>{0}),
                .node = tree_root});
    while (!queue.empty()) {
        auto [code, cur_node] = queue.front();
        queue.pop();
        if (auto left_son = cur_node->GetLeftSon(); left_son) {
            code.push_back(0);
            queue.push({.code = code, .node = left_son});
            code.pop_back();
        }
        if (auto right_son = cur_node->GetRightSon(); right_son) {
            code.push_back(1);
            queue.push({.code = code, .node = right_son});
            code.pop_back();
        }
        if (auto value = cur_node->GetValue(); value != BinaryTree::NO_VALUE) {
            codebook.push_back({.character = value, .code = std::move(code)});
        }
    }
    return codebook;
}

void Archiver::CompressorImpl::CanonizeCodebook(Codebook& codebook) {
    std::sort(codebook.begin(), codebook.end(), [](const CodeWord& a, const CodeWord& b) {
        return std::forward_as_tuple(a.code.size(), a.character)
             < std::forward_as_tuple(b.code.size(), b.character);
    });

    std::fill(codebook[0].code.begin(), codebook[0].code.end(), 0);
    for (size_t i = 1; i < codebook.size(); ++i) {
        size_t size = codebook[i].code.size();

        // effectively add 1.
        if (std::all_of(codebook[i - 1].code.begin(), codebook[i - 1].code.end(), [](bool b) { return b; })) {
            codebook[i].code.assign(1, 1);
            for (size_t j = 1; j < size; ++j) {
                codebook[i].code.push_back(0);
            }
        } else {
            codebook[i].code = codebook[i - 1].code;
            auto it = codebook[i].code.rbegin();
            while (it != codebook[i].code.rend() && *it == 1) {
                *it = 0;
                ++it;
            }
            *it = 1;
        }

        while (codebook[i].code.size() != size) {
            codebook[i].code.push_back(0);
        }
    }
}

CodebookData Archiver::CompressorImpl::GetCodebookData(Codebook codebook) {
    if (codebook.empty()) {
        return {};
    }

    auto max_bit_count = codebook.back().code.size();
    CodebookData codebook_data{.word_count_by_bit_count = std::vector<size_t>(max_bit_count),
                               .characters = std::vector<unsigned short>(codebook.size())};
    for (size_t i = 0; i < codebook.size(); ++i) {
        codebook_data.characters[i] = codebook[i].character;
        ++codebook_data.word_count_by_bit_count[codebook[i].code.size() - 1];
    }

    return codebook_data;
}

EncodingTable Archiver::CompressorImpl::GetEncodingTable(Codebook codebook) {
    EncodingTable encoding_table;
    for (auto& code_word : codebook) {
        encoding_table[code_word.character] = std::move(code_word.code);
    }
    return encoding_table;
}

void Archiver::CompressorImpl::Encode(InputBitStream& in, const EncodingTable& encoding_table,
                                      OutputBitStream& out, ControlCharacters last_character) {
    while (in.Good()) {
        auto character = in.ReadBits(8);
        character <<= 1;
        out.WriteBits(encoding_table.at(character));
    }
    out.WriteBits(encoding_table.at(static_cast<unsigned short>(last_character)));
}
