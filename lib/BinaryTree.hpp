#pragma once

#include <limits>

namespace Archiver {
class BinaryTree {
public:
    static const unsigned short NO_VALUE = std::numeric_limits<unsigned short>::max();

    explicit BinaryTree(unsigned short value = NO_VALUE);
    BinaryTree(const BinaryTree& other) = delete;
    BinaryTree& operator=(const BinaryTree& other) = delete;

    unsigned short GetValue() const;

    BinaryTree* GetLeftSon() const;
    void SetLeftSon(BinaryTree* left_son);

    BinaryTree* GetRightSon() const;
    void SetRightSon(BinaryTree* right_son);

    ~BinaryTree();

private:
    unsigned short value_;
    BinaryTree* left_son_ = nullptr;
    BinaryTree* right_son_ = nullptr;
};
}  // namespace Archiver
