// The priority queue is implemented as a binary heap.

#pragma once

#include <vector>

namespace Archiver {
template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue() = default;

    explicit PriorityQueue(const Compare& compare) : comp_(compare) { }

    PriorityQueue(const PriorityQueue& other) = default;

    PriorityQueue(PriorityQueue&& other) = default;
    PriorityQueue& operator=(const PriorityQueue& other) = default;
    PriorityQueue& operator=(PriorityQueue&& other) = default;

    const T& top() const {
        return data_[0];
    }

    bool empty() const {
        return data_.empty();
    }

    size_t size() const {
        return data_.size();
    }

    void push(const T& value) {
        data_.push_back(value);
        sift_up(data_.size() - 1);
    }

    void pop() {
        std::swap(data_[0], data_.back());
        data_.pop_back();
        sift_down(0);
    }

private:
    void sift_up(size_t indx) {
        while (indx != 0 && comp_(data_[(indx - 1) / 2], data_[indx])) {
            std::swap(data_[indx], data_[(indx - 1) / 2]);
            indx = (indx - 1) / 2;
        }
    }

    void sift_down(size_t indx) {
        while (2 * indx + 1 < data_.size()) {
            size_t left = 2 * indx + 1;
            size_t right = 2 * indx + 2;

            size_t swap_with = left;
            if (right < data_.size() && comp_(data_[left], data_[right])) {
                swap_with = right;
            }
            if (!comp_(data_[indx], data_[swap_with])) {
                break;
            }
            std::swap(data_[indx], data_[swap_with]);
            indx = swap_with;
        }
    }

private:
    std::vector<T> data_;
    Compare comp_;
};
}  // namespace Archiver
