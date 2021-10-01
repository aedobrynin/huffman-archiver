#include <gtest/gtest.h>

#include <vector>

#include "Archiver.hpp"

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
        Codebook codebook{{.symbol = 10, .bit_count = 1}};
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

        Codebook codebook{{.symbol = 256, .bit_count = 1},
                          {.symbol = 0, .bit_count = 2},
                          {.symbol = 1, .bit_count = 3},
                          {.symbol = 2, .bit_count = 3}};
        tests.push_back({.tree = std::move(tree_root), .expected = codebook});
    }

    for (auto& test : tests) {
        // We do not care about order of elements in the codebook.
        auto IsCodeWordLess = [](const CodeWord& a, const CodeWord& b) {
            return std::tie(a.symbol, a.bit_count) < std::tie(b.symbol, b.bit_count);
        };
        std::sort(test.expected.begin(), test.expected.end(), IsCodeWordLess);
        auto result = GetCodebook(test.tree.get());
        std::sort(result.begin(), result.end(), IsCodeWordLess);
        ASSERT_EQ(test.expected, result);
    }
}

