#pragma once

#include <memory>
#include <iterator>
#include "./iterator.hpp"
#include <memory_resource>

#include <algorithm>
#include <stdexcept>


template<class T>
class DynArray {
private:
    struct DynArrayDeleter {
        std::pmr::polymorphic_allocator<T> allocator;
        size_t capacity;

        DynArrayDeleter(std::pmr::polymorphic_allocator<T> alloc, size_t capacity) : allocator(alloc), capacity(capacity) {}

        void operator()(T* ptr) const {
            if (ptr) {
                allocator.deallocate(ptr, capacity);
            }
        }
    };

    std::pmr::polymorphic_allocator<T> _allocator;
    std::unique_ptr<T[], DynArrayDeleter> _data;

    size_t _size;
    size_t _capacity;

public:
    using iterator = DynArrayIterator<T>;               // итератор для изменения данных
    using const_iterator =  DynArrayIterator<const T>;  // итератор для R_ONLY
    
    DynArray(std::pmr::polymorphic_allocator<T> alloc = {});
    DynArray(size_t capacity, std::pmr::polymorphic_allocator<T> alloc = {});

    DynArray(const DynArray& other);
    DynArray& operator=(const DynArray& other);
    ~DynArray() {clear();}

    void reserve(size_t new_cap);
    void swap(DynArray& other) noexcept;
    void push_back(const T& val);

    iterator begin() {return iterator(_data.get());}
    const_iterator begin() const {return const_iterator(_data.get());}
    iterator end() {return iterator(_data.get() + _size);}
    const_iterator end() const {return const_iterator(_data.get() + _size);}

    // доступ по индексу
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t size() const {return _size;}
    size_t capacity() const {return _capacity;}
    bool isEmpty() const {return _size == 0;}

    void clear();
};


template<class T>
DynArray<T>::DynArray(std::pmr::polymorphic_allocator<T> alloc) : _allocator(alloc), _data(nullptr, DynArrayDeleter(alloc, 0)), _size(0), _capacity(0) {}

template<class T>
DynArray<T>::DynArray(size_t capacity, std::pmr::polymorphic_allocator<T> alloc) : _allocator(alloc), _size(0), _capacity(capacity) {
    if (_capacity > 0) {
        T* raw_data = _allocator.allocate(_capacity);
        for (size_t i = 0; i < _capacity; ++i) {
            _allocator.construct(raw_data + i);
        }

        _data = std::unique_ptr<T[], DynArrayDeleter> (raw_data, DynArrayDeleter(_allocator, _capacity));
    }
}


template<class T>
DynArray<T>::DynArray(const DynArray& other) : _allocator(other._allocator), _size(other._size), _capacity(other._capacity) {
    if (_capacity > 0) {
        T* raw_data = _allocator.allocate(_capacity);
        for (size_t i = 0; i < _size; ++i) {
            _allocator.construct(raw_data + i, other._data[i]);
        }

        _data = std::unique_ptr<T[], DynArrayDeleter> (raw_data, DynArrayDeleter(_allocator, _capacity));
    }
}

template<class T>
DynArray<T>& DynArray<T>::operator=(const DynArray& other) {
    // if (this != &other) {
    //     clear();
    //     _data.reset();

    //     _size = other._size;
    //     _capacity = other._capacity;
    //     _allocator = other._allocator;

    //     if (_capacity > 0) {
    //         T* raw_data = _allocator.allocate(_capacity);
    //         for (size_t i = 0; i < _capacity; ++i) {
    //             _allocator.construct(raw_data + i);
    //         }

    //         _data = std::unique_ptr<T[], DynArrayDeleter> (raw_data, DynArrayDeleter(_allocator, _capacity));
    //     }
    // }
    // return *this;
    DynArray temp(other);
    swap(*this, temp);
    return *this;
}


template<class T>
void DynArray<T>::reserve(size_t new_cap) {
    if (new_cap <= _capacity) return;

    T* new_raw_data = _allocator.allocate(new_cap);
    // копируем старые эл-ты
    for (size_t i = 0; i < _size; ++i) {
        _allocator.construct(new_raw_data + i, std::move(_data[i]));
        _allocator.destroy(_data.get() + i);
    }
    // создаём новые
    for (size_t i = _size; i < new_cap; ++i) {
        _allocator.construct(new_raw_data + i);
    }
    _data = std::unique_ptr<T[], DynArrayDeleter> (new_raw_data, DynArrayDeleter(_allocator, new_cap));
    _capacity = new_cap;
}

template<class T>
void DynArray<T>::swap(DynArray& other) noexcept { 
    using std::swap;
    swap(_data, other._data);
    swap(_size, other._size);
    swap(_capacity, other._capacity);
    swap(_allocator, other._allocator);
}

template<class T>
void DynArray<T>::push_back(const T& val) {
    if (_size >= _capacity) {
        reserve(_capacity == 0 ? 1 : _capacity * 2);
    }
    _allocator.construct(_data.get() + _size, val);
    ++_size;
}


template<class T>
T& DynArray<T>::operator[](size_t index) {
    if (index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _data[index];
}

template<class T>
const T& DynArray<T>::operator[](size_t index) const {
    if (index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _data[index];
}

template<class T>
void DynArray<T>::clear() {
    for (size_t i = 0; i < _size; ++i) {
        _allocator.destroy(_data.get() + i);
    }
    _size = 0;
}