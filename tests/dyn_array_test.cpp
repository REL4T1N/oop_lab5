#include <gtest/gtest.h>
#include "../include/dyn_array.hpp"
#include "../include/memory_resource.hpp"

TEST(DynArrayTest, EmptyCreation) {
    DynArray<int> arr;
    EXPECT_TRUE(arr.isEmpty());
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 0);
}

TEST(DynArrayTest, PushBack) {
    DynArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(DynArrayTest, CopyConstructor) {
    DynArray<int> arr1;
    arr1.push_back(1);
    arr1.push_back(2);
    
    DynArray<int> arr2 = arr1;
    EXPECT_EQ(arr2.size(), 2);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
}

TEST(DynArrayTest, AssignmentOperator) {
    DynArray<int> arr1;
    arr1.push_back(1);
    arr1.push_back(2);
    
    DynArray<int> arr2;
    arr2 = arr1;
    EXPECT_EQ(arr2.size(), 2);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
}

TEST(DynArrayTest, Iterator) {
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

TEST(DynArrayTest, RangeBasedFor) {
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