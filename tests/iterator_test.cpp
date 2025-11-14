#include <gtest/gtest.h>
#include "../include/iterator.hpp"
#include "../include/dyn_array.hpp"

TEST(IteratorTest, BasicIteration) {
    DynArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    
    int sum = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(IteratorTest, RangeBasedFor) {
    DynArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    
    int sum = 0;
    for (int val : arr) {
        sum += val;
    }
    EXPECT_EQ(sum, 6);
}