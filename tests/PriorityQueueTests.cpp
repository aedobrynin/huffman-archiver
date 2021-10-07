#include <gtest/gtest.h>

#include "PriorityQueue.hpp"

#include <queue>
#include <random>

using namespace Archiver;

TEST(PriorityQueueTests, CorrectSizeMethod) {
    PriorityQueue<int> priority_queue;

    ASSERT_EQ(priority_queue.size(), 0);
    for (int i = 0; i < 10; ++i) {
        priority_queue.push(i);
        ASSERT_EQ(priority_queue.size(), i + 1);
    }
}

TEST(PriorityQueueTests, CorrectCopyConstruction) {
    PriorityQueue<int> a;
    for (int i = 0; i < 10; ++i) {
        a.push(i);
    }

    PriorityQueue<int> b(a);

    ASSERT_EQ(a.size(), b.size());
    while (!a.empty()) {
        auto a_el = a.top();
        auto b_el = b.top();
        a.pop();
        b.pop();
        ASSERT_EQ(a_el, b_el);
    }
}

TEST(PriorityQueueTests, CorrectCopyAssignment) {
    PriorityQueue<int> a;
    PriorityQueue<int> b;
    for (int i = 0; i < 10; ++i) {
        a.push(i);
    }
    for (int i = 0; i < 100; ++i) {
        b.push(100);
    }

    b = a;

    ASSERT_EQ(a.size(), b.size());
    while (!a.empty()) {
        auto a_el = a.top();
        auto b_el = b.top();
        a.pop();
        b.pop();
        ASSERT_EQ(a_el, b_el);
    }
}

TEST(PriorityQueueTests, CorrectMoveConstruction) {
    PriorityQueue<int> a;
    for (int i = 0; i < 10; ++i) {
        a.push(i);
    }

    PriorityQueue<int> b(std::move(a));

    ASSERT_TRUE(a.empty());
    ASSERT_EQ(b.size(), 10);
    for (int expected = 9; expected >= 0; --expected) {
        auto b_el = b.top();
        b.pop();
        ASSERT_EQ(b_el, expected);
    }
}

TEST(PriorityQueueTests, CorrectMoveAssignment) {
    PriorityQueue<int> a;
    PriorityQueue<int> b;
    for (int i = 0; i < 10; ++i) {
        a.push(i);
    }
    for (int i = 0; i < 100; ++i) {
        b.push(100);
    }

    b = std::move(a);

    ASSERT_TRUE(a.empty());
    ASSERT_EQ(b.size(), 10);
    for (int expected = 9; expected >= 0; --expected) {
        auto b_el = b.top();
        b.pop();
        ASSERT_EQ(b_el, expected);
    }
}

TEST(PriorityQueueTests, CorrectUseOfCompareTemplateArgument) {
    PriorityQueue<int, std::greater<int>> priority_queue;
    for (int i = 0; i < 10; ++i) {
        priority_queue.push(i);
    }

    for (int i = 0; i < 10; ++i) {
        auto elem = priority_queue.top();
        priority_queue.pop();

        ASSERT_EQ(elem, i);
    }
}

TEST(PriorityQueueTests, CorrectPushAndPop) {
    std::priority_queue<int> correct;
    PriorityQueue<int> own;

    std::mt19937 rng(200);
    std::uniform_int_distribution<> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (int i = 0; i < 10000; ++i) {
        auto elem = distr(rng);
        correct.push(elem);
        own.push(elem);
    }
    ASSERT_EQ(correct.size(), own.size());
    while (!correct.empty()) {
        auto correct_el = correct.top();
        correct.pop();
        auto own_el = own.top();
        own.pop();
        ASSERT_EQ(correct_el, own_el);
    }
}
