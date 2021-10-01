#pragma once

namespace Archiver {
class BinaryTree {
public:
    explicit BinaryTree(unsigned short value = 0);
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
