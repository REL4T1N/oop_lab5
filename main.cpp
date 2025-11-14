#include <iostream>
#include "./include/memory_resource.hpp"
#include "./include/dyn_array.hpp"

struct ComplexType {
    int id;
    double value;
    std::string name;

    ComplexType(int id = 0, double value = 0.0, const std::string& name = "") 
        : id(id), value(value), name(name) {}
};

int main() {
    VectorMemoryResource resource;
    
    // Демонстрация с простыми типами (int)
    {
        std::cout << "=== Demonstration with int ===" << std::endl;
        std::pmr::polymorphic_allocator<int> alloc(&resource);
        DynArray<int> arr(alloc);
        
        for (int i = 0; i < 5; ++i) {
            arr.push_back(i * 10);
        }
        
        std::cout << "Array: ";
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        resource.print_stats();
    }
    
    // Демонстрация со сложными типами
    {
        std::cout << "\n=== Demonstration with ComplexType ===" << std::endl;
        std::pmr::polymorphic_allocator<ComplexType> alloc(&resource);
        DynArray<ComplexType> arr(alloc);
        
        arr.push_back(ComplexType(1, 3.14, "Pi"));
        arr.push_back(ComplexType(2, 2.71, "E"));
        arr.push_back(ComplexType(3, 1.41, "sqrt(2)"));
        
        std::cout << "Complex array:\n";
        for (const auto& item : arr) {
            std::cout << "  id: " << item.id << ", value: " << item.value 
                      << ", name: " << item.name << std::endl;
        }
        
        resource.print_stats();
    }
    
    // Демонстрация переиспользования памяти
    {
        std::cout << "\n=== Memory Reuse Demonstration ===" << std::endl;
        std::pmr::polymorphic_allocator<int> alloc(&resource);
        
        // Создаем и удаляем несколько массивов
        DynArray<int>* arr1 = new DynArray<int>(alloc);
        for (int i = 0; i < 3; ++i) {
            arr1->push_back(i);
        }
        delete arr1;
        
        // Новый массив должен переиспользовать память
        DynArray<int> arr2(alloc);
        for (int i = 10; i < 13; ++i) {
            arr2.push_back(i);
        }
        
        std::cout << "Reused memory array: ";
        for (auto val : arr2) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        
        resource.print_stats();
    }
    
    return 0;
}