#include "../include/memory_resource.hpp"


VectorMemoryResource::VectorMemoryResource(std::pmr::memory_resource* upstream) : _upstream(upstream ? upstream : std::pmr::new_delete_resource()) {
            std::cout << "VectorMemoryResource created\n";
}

VectorMemoryResource::~VectorMemoryResource() {
    for (const auto& alloc : _allocations) {
        if (alloc.in_use) {
            std::cout << "WARNING: Block still in use at desctruction: " << alloc.size << " bytes at " << alloc.ptr << std::endl;
        }
        _upstream->deallocate(alloc.ptr, alloc.size, alloc.alignment);
    }
}


void* VectorMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    for (auto& alloc : _allocations) {
        if (!alloc.in_use && alloc.size >= bytes && alloc.alignment >= alignment) {
            alloc.in_use = true;
            std::cout << "[Memory] Reused " << bytes << " bytes\n";
            return alloc.ptr;
        }
    }

    void* ptr = _upstream->allocate(bytes, alignment);
    _allocations.emplace_back(ptr, bytes, alignment);
    return ptr;
}

void VectorMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    for (auto& alloc : _allocations) {
        if (alloc.ptr == p) {
            if (!alloc.in_use) {
                throw std::logic_error("Double deallocation detected");
            }
            alloc.in_use = false;
            return;
        }
    }
    throw std::logic_error("Deallocation of unknown block");
}


size_t VectorMemoryResource::allocation_count() const {return _allocations.size();}

size_t VectorMemoryResource::used_blocks() const {
    return std::count_if(_allocations.begin(), _allocations.end(), [](const Allocation& alloc) {return alloc.in_use;});
}

size_t VectorMemoryResource::total_allocated() const {
    size_t total = 0;
    for (const auto& alloc : _allocations) {
        total += alloc.size;
    }
    return total;
}


void VectorMemoryResource::print_stats() const {
    std::cout << "[VectorMemoryResource] Stats:\nTotal blocks: " << allocation_count() << "\nUsed blocks: " << used_blocks() << "\nFree blocks: " << (allocation_count() - used_blocks()) << "\nTotal allocated: " << total_allocated() << " bytes" << std::endl; 
}