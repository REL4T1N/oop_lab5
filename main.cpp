#include <iostream>
#include <string>
#include "../include/dyn_array.hpp"
#include "../include/memory_resource.hpp"

// Сложный тип для демонстрации
struct Person {
    std::string name;
    int age;
    double salary;
    
    Person(const std::string& n = "", int a = 0, double s = 0.0) 
        : name(n), age(a), salary(s) {}
    
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << "Person{name: \"" << p.name << "\", age: " << p.age 
           << ", salary: " << p.salary << "}";
        return os;
    }
};

void demo_simple_types() {
    std::cout << "=== Демонстрация с простыми типами (int) ===" << std::endl;
    
    VectorMemoryResource resource;
    std::pmr::polymorphic_allocator<int> alloc(&resource);
    
    DynArray<int> numbers(alloc);
    
    for (int i = 0; i < 10; ++i) {
        numbers.push_back(i * i);
    }
    
    // вывод через итератор
    std::cout << "Numbers: ";
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // вывод через range-based for
    std::cout << "Range-based for: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    resource.print_stats();
}

void demo_complex_types() {
    std::cout << "\n=== Демонстрация со сложными типами (Person) ===" << std::endl;
    
    VectorMemoryResource resource;
    std::pmr::polymorphic_allocator<Person> alloc(&resource);
    
    DynArray<Person> people(alloc);
    
    people.push_back(Person("Alice", 30, 50000.0));
    people.push_back(Person("Bob", 25, 45000.0));
    people.push_back(Person("Charlie", 35, 60000.0));
    
    std::cout << "People:" << std::endl;
    for (auto it = people.begin(); it != people.end(); ++it) {
        std::cout << "  " << *it << std::endl;
    }
    
    // доступ по индексу
    std::cout << "Second person: " << people[1] << std::endl;
    
    resource.print_stats();
}

void demo_memory_reuse() {
    std::cout << "\n=== Демонстрация переиспользования памяти ===" << std::endl;
    
    VectorMemoryResource resource;
    
    {
        std::pmr::polymorphic_allocator<int> alloc(&resource);
        DynArray<int> temp(alloc);
        
        for (int i = 0; i < 5; ++i) {
            temp.push_back(i);
        }
        
        std::cout << "До очистки - ";
        resource.print_stats();
    }   // temp выходит из области видимости, память освобождается
    
    std::cout << "После очистки - ";
    resource.print_stats();
    
    // новый массив, который должен переиспользовать память
    std::pmr::polymorphic_allocator<int> alloc(&resource);
    DynArray<int> new_array(alloc);
    
    for (int i = 0; i < 3; ++i) {
        new_array.push_back(i * 10);
    }
    
    std::cout << "После переиспользования - ";
    resource.print_stats();
}

int main() {
    demo_simple_types();
    demo_complex_types();
    demo_memory_reuse();
    
    return 0;
}