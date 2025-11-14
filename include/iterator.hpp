#pragma once

// #include <cstddef>
#include <memory>
#include <iterator>


template<typename T>
class DynArrayIterator {
private:
    // T* _ptr;
    // size_t _index;
    // size_t _capacity;
    T* _current;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;


    // DynArrayIterator(T* ptr, size_t index, size_t capacity) : _ptr(ptr), _index(index), _capacity(capacity) {}
    DynArrayIterator(T* ptr) : _current(ptr) {}

    reference operator*() const {return *_current;} //{return _ptr[_index];}
    pointer operator->() const {return _current;}   //{return &_ptr[_index];}

    
    DynArrayIterator& operator++();    // prefix - возвращает ссылку на уже измененный объект
    DynArrayIterator operator++(int);  // postfix - возвращает копию исходного значения до изменения
    // декремента нет, т.к. forward_iterator

    bool operator==(const DynArrayIterator& other) const {return _current == other._current;}
    bool operator!=(const DynArrayIterator& other) const {return !(*this == other);}
};


template<typename T>
DynArrayIterator<T>& DynArrayIterator<T>::operator++() {
    ++_current;
    return *this;
}

template<typename T>
DynArrayIterator<T> DynArrayIterator<T>::operator++(int) {
    DynArrayIterator temp = *this;
    ++_current;
    return temp;
}