#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <memory_resource>
#include <algorithm>    // зачем??


class VectorMemoryResource : public std::pmr::memory_resource {
private:
    struct Allocation {
        void* ptr;
        size_t size;
        size_t alignment;
        bool in_use;

        Allocation(void* p, size_t s, size_t a) : ptr(p), size(s), alignment(a), in_use(true) {}
    };

    std::vector<Allocation> _allocations;
    std::pmr::memory_resource* _upstream;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {return this == &other;}


public:
    VectorMemoryResource(std::pmr::memory_resource* upstream = nullptr);

    virtual ~VectorMemoryResource();

    // запрет копирования и перемещения
    VectorMemoryResource(const VectorMemoryResource&) = delete;
    VectorMemoryResource& operator=(const VectorMemoryResource&) = delete;

    size_t allocation_count() const;
    size_t used_blocks() const;
    size_t total_allocated() const;

    void print_stats() const;
};
