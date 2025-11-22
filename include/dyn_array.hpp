#pragma once

#include <memory>
#include <iterator>
#include "iterator.hpp"
#include <memory_resource>
#include <algorithm>
#include <stdexcept>

template<class T>
class DynArray {
private:
    std::pmr::polymorphic_allocator<T> _allocator;
    std::unique_ptr<T[]> _data;  // default deleter - только освобождает память, не вызывает деструкторы!
    size_t _size;
    size_t _capacity;

    // ручное управление деструкторами
    void destroy_elements();
    void cleanup();


public:
    using iterator = DynArrayIterator<T>;
    using const_iterator = DynArrayIterator<const T>;
    
    DynArray(std::pmr::polymorphic_allocator<T> alloc = {});
    DynArray(size_t capacity, std::pmr::polymorphic_allocator<T> alloc = {});

    DynArray(const DynArray& other);
    DynArray& operator=(const DynArray& other);
    ~DynArray() {cleanup();}

    void reserve(size_t new_cap);
    void swap(DynArray& other) noexcept;
    void push_back(const T& val);
    void push_back(T&& val);

    iterator begin() { return iterator(_data.get()); }
    const_iterator begin() const { return const_iterator(_data.get()); }
    iterator end() { return iterator(_data.get() + _size); }
    const_iterator end() const { return const_iterator(_data.get() + _size); }

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    bool isEmpty() const { return _size == 0; }

    void clear();
};

template<class T>
void DynArray<T>::destroy_elements() {
    for (size_t i = 0; i < _size; ++i) {
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::destroy(_allocator, _data.get() + i);
    }
}

template<class T>
void DynArray<T>::cleanup() {
    destroy_elements();
    if (_data) {
        _allocator.deallocate(_data.release(), _capacity);
    }
}


template<class T>
DynArray<T>::DynArray(std::pmr::polymorphic_allocator<T> alloc) : _allocator(alloc), _data(nullptr), _size(0), _capacity(0) {}

template<class T>
DynArray<T>::DynArray(size_t capacity, std::pmr::polymorphic_allocator<T> alloc) 
    : _allocator(alloc), _data(nullptr), _size(0), _capacity(0) {
    if (capacity > 0) {
        reserve(capacity);
    }
}


template<class T>
DynArray<T>::DynArray(const DynArray& other) : _allocator(other._allocator), _data(nullptr), _size(0), _capacity(0) {
    if (other._capacity > 0) {
        reserve(other._capacity);
        _size = other._size;
        for (size_t i = 0; i < _size; ++i) {
            std::allocator_traits<std::pmr::polymorphic_allocator<T>>::construct(
                _allocator, _data.get() + i, other._data[i]);
        }
    }
}

template<class T>
DynArray<T>& DynArray<T>::operator=(const DynArray& other) {
    if (this != &other) {
        DynArray temp(other);
        swap(temp);
    }
    return *this;
}


template<class T>
void DynArray<T>::reserve(size_t new_cap) {
    if (new_cap <= _capacity) return;

    T* new_raw_data = _allocator.allocate(new_cap);
            for (size_t i = 0; i < _size; ++i) {
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::construct(
            _allocator, new_raw_data + i, std::move_if_noexcept(_data[i]));
        std::allocator_traits<std::pmr::polymorphic_allocator<T>>::destroy(
            _allocator, _data.get() + i);
    }
    
    if (_data) {
        _allocator.deallocate(_data.release(), _capacity);
    }
    _data.reset(new_raw_data);
    _capacity = new_cap;
}

template<class T>
void DynArray<T>::swap(DynArray& other) noexcept { 
    using std::swap;
    swap(_data, other._data);
    swap(_size, other._size);
    swap(_capacity, other._capacity);
    // std::pmr::polymorphic_allocator не поддерживает операцию swap (выведенно часовым поиском проблемы)
    // swap(_allocator, other._allocator);
}

template<class T>
void DynArray<T>::push_back(const T& val) {
    if (_size >= _capacity) {
        reserve(_capacity == 0 ? 1 : _capacity * 2);
    }
    std::allocator_traits<std::pmr::polymorphic_allocator<T>>::construct(
        _allocator, _data.get() + _size, val);
    ++_size;
}

template<class T>
void DynArray<T>::push_back(T&& val) {
    if (_size >= _capacity) {
        reserve(_capacity == 0 ? 1 : _capacity * 2);
    }
    std::allocator_traits<std::pmr::polymorphic_allocator<T>>::construct(
        _allocator, _data.get() + _size, std::move(val));
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
    destroy_elements();
    _size = 0;
}