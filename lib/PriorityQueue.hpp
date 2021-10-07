#pragma once

#include <ctime>
#include <random>

namespace Archiver {
template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue()
        : comp_()
        , rand_rng_(static_cast<size_t>(std::time(nullptr)))
        , rand_distr_(std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::max()) { }

    explicit PriorityQueue(const Compare& compare) : comp_(compare) { }
    PriorityQueue(const PriorityQueue& other)
        : size_(other.size_)
        , comp_()
        , rand_rng_(static_cast<size_t>(std::time(nullptr)))
        , rand_distr_(std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::max()) {
        tree_ = (other.tree_ ? other.tree_->get_copy() : nullptr);
    }

    PriorityQueue(PriorityQueue&& other) {
        std::swap(tree_, other.tree_);
        std::swap(size_, other.size_);
        std::swap(rand_rng_, other.rand_rng_);
        std::swap(rand_distr_, other.rand_distr_);
    }

    PriorityQueue& operator=(const PriorityQueue& other) {
        delete tree_;
        size_ = other.size_;
        tree_ = (other.tree_ ? other.tree_->get_copy() : nullptr);
        return *this;
    }

    PriorityQueue& operator=(PriorityQueue&& other) {
        delete tree_;
        tree_ = nullptr;
        size_ = 0;
        std::swap(tree_, other.tree_);
        std::swap(size_, other.size_);
        std::swap(rand_rng_, other.rand_rng_);
        std::swap(rand_distr_, other.rand_distr_);
        return *this;
    }

    const T& top() const {
        Node* cur = tree_;
        while (cur->right_son) {
            cur = cur->right_son;
        }
        return cur->value;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    void push(const T& value) {
        tree_ = insert(tree_, value, rand_distr_(rand_rng_));
        ++size_;
    }

    void pop() {
        if (!tree_->right_son) {
            Node* new_tree = tree_->left_son;
            tree_->left_son = nullptr;
            delete tree_;
            tree_ = new_tree;
        } else {
            Node* cur = tree_;
            while (cur->right_son && cur->right_son->right_son) {
                cur = cur->right_son;
            }
            Node* new_right_son = cur->right_son->left_son;
            cur->right_son->left_son = nullptr;
            delete cur->right_son;
            cur->right_son = new_right_son;
        }
        --size_;
    }

    ~PriorityQueue() {
        delete tree_;
    }

private:
    struct Node {
        Node* left_son;
        Node* right_son;
        T value;
        size_t internal_priority;

        explicit Node(const T& val, size_t priority)
            : left_son(nullptr), right_son(nullptr), value(val), internal_priority(priority) { }

        Node* get_copy() {
            Node* copy = new Node(value, internal_priority);
            if (left_son) {
                copy->left_son = left_son->get_copy();
            }
            if (right_son) {
                copy->right_son = right_son->get_copy();
            }
            return copy;
        }

        ~Node() {
            delete left_son;
            delete right_son;
        }
    };

    std::pair<Node*, Node*> split(Node * node, const T& key) {
        if (!node) {
            return {nullptr, nullptr};
        }

        if (comp_(node->value, key)) {
            auto right_son_split = split(node->right_son, key);
            node->right_son = right_son_split.first;
            return {node, right_son_split.second};
        } else {
            auto left_son_split = split(node->left_son, key);
            node->left_son = left_son_split.second;
            return {left_son_split.first, node};
        }
    }

    Node* merge(Node* a, Node* b) {
        if (!a) {
            return b;
        }

        if (!b) {
            return a;
        }

        if (a->internal_priority > b->internal_priority) {
            a->right_son = merge(a->right_son, b);
            return a;
        } else {
            b->left_son = merge(a, b->left_son);
            return b;
        }
    }

    Node* insert(Node* tree, const T& val, size_t priority) {
        Node* new_node = new Node(val, priority);

        auto tree_split = split(tree, val);
        return merge(merge(tree_split.first, new_node), tree_split.second);
    }

private:
    Node* tree_ = nullptr;
    size_t size_ = 0;

    Compare comp_;

    std::mt19937 rand_rng_;
    std::uniform_int_distribution<size_t> rand_distr_;
};
}  // namespace Archiver
