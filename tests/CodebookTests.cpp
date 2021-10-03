#include <gtest/gtest.h>

#include <vector>

#include "Compress.hpp"

using namespace Archiver;

TEST(CodebookTests, CorrectGetCodebook) {
    struct TestData {
        std::unique_ptr<BinaryTree> tree;
        Codebook expected;
    };
    std::vector<TestData> tests;

    {
        std::unique_ptr<BinaryTree> tree_root = nullptr;
        Codebook codebook{};
        tests.push_back({.tree = std::move(tree_root), .expected = codebook});
    }

    {
        auto tree_root = std::make_unique<BinaryTree>(10);
        Codebook codebook{{.character = 10, .code = {0}}};
        tests.push_back({.tree = std::move(tree_root), .expected = codebook});
    }

    {
        auto tree_root = std::make_unique<BinaryTree>();
        tree_root->SetLeftSon(new BinaryTree(256));
        auto right_son = new BinaryTree();
        tree_root->SetRightSon(right_son);
        right_son->SetLeftSon(new BinaryTree(0));
        auto right_right_son = new BinaryTree();
        right_son->SetRightSon(right_right_son);
        right_right_son->SetLeftSon(new BinaryTree(1));
        right_right_son->SetRightSon(new BinaryTree(2));

        Codebook codebook{{.character = 256, .code = {0}},
                          {.character = 0, .code = {1, 0}},
                          {.character = 1, .code = {1, 1, 0}},
                          {.character = 2, .code = {1, 1, 1}}};
        tests.push_back({.tree = std::move(tree_root), .expected = codebook});
    }

    for (auto& test : tests) {
        // We do not care about order of elements in the codebook.
        auto IsCodeWordLess = [](const CodeWord& a, const CodeWord& b) {
            return std::forward_as_tuple(a.character, a.code.size())
                 < std::forward_as_tuple(b.character, b.code.size());
        };
        std::sort(test.expected.begin(), test.expected.end(), IsCodeWordLess);
        auto result = GetCodebook(test.tree.get());
        std::sort(result.begin(), result.end(), IsCodeWordLess);
        ASSERT_EQ(test.expected, result);
    }
}

TEST(CodebookTests, CorrectGetCodebookData) {
    struct TestData {
        Codebook codebook;
        CodebookData expected;
    };
    std::vector<TestData> tests;

    {
        Codebook codebook{};
        CodebookData expected{};
        tests.push_back({.codebook = codebook, .expected = expected});
    }

    {
        Codebook codebook{{.character = 10, .code = {0}}};
        CodebookData expected{.word_count_by_bit_count = {1},
                                   .characters = {10}};
        tests.push_back({.codebook = codebook, .expected = expected});
    }

    {
        Codebook codebook{{.character = 256, .code = {0}},
                          {.character = 0, .code = {1, 0}},
                          {.character = 1, .code = {1, 1, 0}},
                          {.character = 2, .code = {1, 1, 1}}};
        CodebookData expected{.word_count_by_bit_count = {1, 1, 2},
                                   .characters = {256, 0, 1, 2}};
        tests.push_back({.codebook = codebook, .expected = expected});
    }

    ASSERT_FALSE(tests.empty());

    for (const auto& test : tests) {
        ASSERT_EQ(GetCodebookData(test.codebook), test.expected);
    }
}
