#include "BinaryTree.hpp"

using namespace Archiver;

BinaryTree::BinaryTree(unsigned short value) : value_(value) { }

unsigned short BinaryTree::GetValue() const {
    return value_;
}

BinaryTree* BinaryTree::GetLeftSon() const {
    return left_son_;
}

void BinaryTree::SetLeftSon(BinaryTree* left_son) {
    left_son_ = left_son;
}

BinaryTree* BinaryTree::GetRightSon() const {
    return right_son_;
}

void BinaryTree::SetRightSon(BinaryTree* right_son) {
    right_son_ = right_son;
}

BinaryTree::~BinaryTree() {
    delete left_son_;
    delete right_son_;
}
