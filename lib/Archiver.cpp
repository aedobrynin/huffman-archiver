#include "Archiver.hpp"

#include <fstream>
#include <functional>
#include <queue>
#include <sstream>

using namespace Archiver;

void Archiver::Compress(const std::string& archive_name, const std::vector<std::string>& files_to_archive) {
    std::ofstream fout(archive_name);
    OutputBitStream obitstream(fout);
    for (size_t i = 0; i < files_to_archive.size(); ++i) {
        std::ifstream fin(files_to_archive[i]);
        InputBitStream ibitstream(fin);
        CompressFile(files_to_archive[i], ibitstream, obitstream, i + 1 == files_to_archive.size());
    }
}

void Archiver::CompressFile(const std::string& filename, InputBitStream& in, OutputBitStream& out, bool is_last) {
    std::vector<ControlCharacters> additional_characters{
        ControlCharacters::FILENAME_END,
        (is_last ? ControlCharacters::ARCHIVE_END : ControlCharacters::ONE_MORE_FILE),
    };

    auto frequency_list = GetFrequencyList(filename, in, additional_characters);
    auto binary_tree = GetBinaryTree(frequency_list);
    auto codebook = GetCodebook(binary_tree);
    auto canonical_codebook = GetCanonicalCodebook(codebook);
}

FrequencyList Archiver::GetFrequencyList(const std::string& filename, InputBitStream& ibitstream,
                                         const std::vector<ControlCharacters>& additional_characters) {
    FrequencyList frequency_list;
    frequency_list.fill(0);

    std::stringstream filename_sstream(filename);
    InputBitStream filename_ibitstream(filename_sstream);
    while (filename_ibitstream.good()) {
        auto character = filename_ibitstream.ReadBits(8);
        ++frequency_list[character];
    }

    while (ibitstream.good()) {
        auto character = ibitstream.ReadBits(8);
        ++frequency_list[character];
    }

    for (auto additional_character : additional_characters) {
        unsigned short character = 0;
        for (size_t i = 0; i < 9; ++i) {
            character <<= 1;
            character |= (static_cast<unsigned short>(additional_character) >> i) & 1;
        }
        ++frequency_list[character];
    }

    return frequency_list;
}

BinaryTree* Archiver::GetBinaryTree(const FrequencyList& frequency_list) {
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
        priority_queue.push({.occurance_count = frequency_list[i], .node = new BinaryTree(i)});
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
        priority_queue.push({.occurance_count = left_son.occurance_count + right_son.occurance_count, .node = parent});
    }

    return priority_queue.top().node;
}

Codebook Archiver::GetCodebook(const BinaryTree* tree_root) {
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

CanonicalCodebook Archiver::GetCanonicalCodebook(Codebook codebook) {
    if (codebook.empty()) {
        return {};
    }

    auto IsLessByBitCount = [](const CodeWord& a, const CodeWord& b) {
        return a.code.size() < b.code.size();
    };
    std::sort(codebook.begin(), codebook.end(), IsLessByBitCount);

    auto max_bit_count = codebook.back().code.size();
    CanonicalCodebook canonical_codebook{.word_count_by_bit_count = std::vector<size_t>(max_bit_count),
                                         .characters = std::vector<unsigned short>(codebook.size())};
    for (size_t i = 0; i < codebook.size(); ++i) {
        canonical_codebook.characters[i] = codebook[i].character;
        ++canonical_codebook.word_count_by_bit_count[codebook[i].code.size() - 1];
    }

    return canonical_codebook;
}
