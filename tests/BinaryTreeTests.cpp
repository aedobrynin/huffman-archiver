#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <vector>

#include "BinaryTree.hpp"
#include "CompressorImpl.hpp"

using namespace Archiver;

TEST(BinaryTreeTests, CorrectGetBinaryTree) {
    std::function<bool(const BinaryTree*, const BinaryTree*)> AreTwoTreesEqual;
    AreTwoTreesEqual = [&AreTwoTreesEqual](const BinaryTree* a, const BinaryTree* b) {
        if (!a || !b) {
            return a == b;
        }

        return a->GetValue() == b->GetValue()
            && AreTwoTreesEqual(a->GetLeftSon(), b->GetLeftSon())
            && AreTwoTreesEqual(a->GetRightSon(), b->GetRightSon());
    };

    struct TestData {
        FrequencyList frequency_list;
        std::unique_ptr<BinaryTree> expected;  // unique_ptr just in case test goes wrong.
    };

    std::vector<TestData> tests;

    {
        FrequencyList frequency_list;
        frequency_list.fill(0);
        std::unique_ptr<BinaryTree> tree_root = nullptr;

        tests.push_back({.frequency_list = frequency_list, .expected = std::move(tree_root)});
    }

    {
        FrequencyList frequency_list;
        frequency_list.fill(0);
        frequency_list[10] = 15;
        auto tree_root = std::make_unique<BinaryTree>(10);

        tests.push_back({.frequency_list = frequency_list, .expected = std::move(tree_root)});
    }

    {
        FrequencyList frequency_list;
        frequency_list.fill(0);

        frequency_list[0] = 10;
        frequency_list[1] = 3;
        frequency_list[2] = 8;
        frequency_list[256] = 15;

        auto tree_root = std::make_unique<BinaryTree>();
        tree_root->SetLeftSon(new BinaryTree(256));
        auto right_son = new BinaryTree();
        tree_root->SetRightSon(right_son);
        right_son->SetLeftSon(new BinaryTree(0));
        auto right_right_son = new BinaryTree();
        right_son->SetRightSon(right_right_son);
        right_right_son->SetLeftSon(new BinaryTree(1));
        right_right_son->SetRightSon(new BinaryTree(2));

        tests.push_back({.frequency_list = frequency_list, .expected = std::move(tree_root)});
    }

    ASSERT_FALSE(tests.empty());

    CompressorImpl compressor_impl;
    for (const auto& test : tests) {
        std::unique_ptr<BinaryTree> result(compressor_impl.GetBinaryTree(test.frequency_list));
        ASSERT_TRUE(AreTwoTreesEqual(test.expected.get(), result.get()));
    }
}
