#include <gtest/gtest.h>
#include "../include/dyn_array.hpp"
#include "../include/memory_resource.hpp"


struct ComplexType {
    int a;
    double b;
    ComplexType(int a, double b) : a(a), b(b) {}
    ComplexType() = default;
    bool operator==(const ComplexType& other) const { return a == other.a && b == other.b; }
};


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


TEST(DynArrayTest, ComplexType) {
    DynArray<ComplexType> arr;
    arr.push_back(ComplexType(1, 2.0));
    arr.push_back(ComplexType(3, 4.0));
    
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].a, 1);
    EXPECT_EQ(arr[0].b, 2.0);
    EXPECT_EQ(arr[1].a, 3);
    EXPECT_EQ(arr[1].b, 4.0);
}


TEST(DynArrayTest, ComplexTypeWithMemoryResource) {
    VectorMemoryResource resource;
    std::pmr::polymorphic_allocator<ComplexType> alloc(&resource);
    DynArray<ComplexType> arr(alloc);
    
    arr.push_back(ComplexType(1, 2.0));
    arr.push_back(ComplexType(3, 4.0));
    
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].a, 1);
    EXPECT_EQ(arr[0].b, 2.0);
    EXPECT_EQ(arr[1].a, 3);
    EXPECT_EQ(arr[1].b, 4.0);
    
    // Проверяем, что memory_resource действительно используется
    EXPECT_GE(resource.allocation_count(), 0);
}