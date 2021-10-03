#pragma once

#include <limits>
#include <vector>

#include "Exceptions.hpp"

namespace Archiver {
class BinaryTree {
public:
    static const unsigned short NO_VALUE = std::numeric_limits<unsigned short>::max();

    explicit BinaryTree(unsigned short value = NO_VALUE);
    BinaryTree(const BinaryTree& other) = delete;
    BinaryTree& operator=(const BinaryTree& other) = delete;

    unsigned short GetValue() const;

    BinaryTree* GetLeftSon();
    const BinaryTree* GetLeftSon() const;
    void SetLeftSon(BinaryTree* left_son);


    BinaryTree* GetRightSon();
    const BinaryTree* GetRightSon() const;
    void SetRightSon(BinaryTree* right_son);

    // The method throws if value is already set.
    template <class ForwardIterator>
    void Append(ForwardIterator first, ForwardIterator last, unsigned short value) {
        BinaryTree* cur = this;
        while (first != last) {
            bool to_the_right = *first;
            if (to_the_right) {
                if (!cur->right_son_) {
                    cur->right_son_ = new BinaryTree();
                }
                cur = cur->right_son_;
            } else {
                if (!cur->left_son_) {
                    cur->left_son_ = new BinaryTree();
                }
                cur = cur->left_son_;
            }
            ++first;
        }
        if (cur->value_ != BinaryTree::NO_VALUE) {
            throw BadFileStructureException("Character has multiple code values.");
        }
        cur->value_ = value;
    }

    ~BinaryTree();

private:
    unsigned short value_;
    BinaryTree* left_son_ = nullptr;
    BinaryTree* right_son_ = nullptr;
};
}  // namespace Archiver
