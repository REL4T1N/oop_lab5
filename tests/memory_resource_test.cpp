#include <gtest/gtest.h>
#include "../include/memory_resource.hpp"


TEST(MemoryResourceTest, BasicAllocation) {
    VectorMemoryResource resource;
    std::pmr::polymorphic_allocator<int> alloc(&resource);
    
    int* ptr = alloc.allocate(1);
    EXPECT_NE(ptr, nullptr);
    alloc.deallocate(ptr, 1);
}


TEST(MemoryResourceTest, MemoryReuse) {
    VectorMemoryResource resource;
    std::pmr::polymorphic_allocator<int> alloc(&resource);
    
    int* ptr1 = alloc.allocate(1);
    alloc.deallocate(ptr1, 1);
    
    int* ptr2 = alloc.allocate(1);
    // Должен переиспользовать память
    EXPECT_EQ(ptr1, ptr2);
    alloc.deallocate(ptr2, 1);
}


TEST(MemoryResourceTest, Stats) {
    VectorMemoryResource resource;
    EXPECT_EQ(resource.allocation_count(), 0);
    EXPECT_EQ(resource.used_blocks(), 0);
    
    std::pmr::polymorphic_allocator<int> alloc(&resource);
    int* ptr = alloc.allocate(1);
    EXPECT_EQ(resource.used_blocks(), 1);
    
    alloc.deallocate(ptr, 1);
    EXPECT_EQ(resource.used_blocks(), 0);
}