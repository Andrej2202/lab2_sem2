#pragma once
#include <Exceptions.hpp>

template <class T>
class DynamicArray{
private:
    T* data_;
    size_t size_;
public:
    using elem = T;
    using pointer = T*;
    using reference = T&;

    DynamicArray(size_t size);
    DynamicArray(pointer items, size_t count);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray(){delete[] data_;}
    
    reference Get(size_t index);
    size_t GetSize(){return size_;}

    void Set(size_t index, T value); 
    void Resize(size_t newSize);
};

#include "DynamicArray.tpp"