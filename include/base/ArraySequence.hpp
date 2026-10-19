#pragma once
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <Exceptions.hpp>
//#include <SuperUniquePointer.hpp>

template <class T> class LinkedList;
template <class T> class ListSequence;


template <class T> 
class ArraySequence : public Sequence<T> { 
private:
    void AppendInternal(T item);
    void PrependInternal(T item);
    void InsertAtInternal(T item, size_t index);
    void ConcatInternal(Sequence<T>* smth);

protected:
    DynamicArray<T>* data_;

    virtual ArraySequence<T>* Clone() const = 0;
    virtual Sequence<T>* Instance() = 0;

public:
    ArraySequence() : data_(new DynamicArray<T>(0)) {}
    ArraySequence(T* items, size_t count);
    ArraySequence(const LinkedList <T> & list);
    ArraySequence(const ArraySequence<T> & array);
    ArraySequence(const Sequence<T>* other) : data_(new DynamicArray<T>(other->GetLength())) {
        for (int i = 0; i < other->GetLength(); i++) {
            data_->Set(i, other->Get(i));
        }
    }
    ArraySequence(const DynamicArray<T>& arr) : data_(new DynamicArray(arr)) {}
    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
    if (this != &other) {
        DynamicArray<T>* newData = new DynamicArray<T>(*other.data_);
        delete data_;
        data_ = newData;
    }
    return *this;
    }
    ~ArraySequence(){delete data_;}

    T GetFirst() const override { return data_->Get(0); }
    T GetLast() const override { return data_->Get(data_->GetSize() - 1); }
    T Get(size_t index) const override { return data_->Get(index); }
    size_t GetLength() const override { return data_->GetSize(); }


    //нуно Mutable/imutable
    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, size_t index) override;
    Sequence<T>* Concat(Sequence<T>* list) override;


    SequenceIterator<T> begin() const override {
        return SequenceIterator<T>(this, 0);
    }

    SequenceIterator<T> end() const override {
        return SequenceIterator<T>(this, this->GetLength());
    }
};


#include "ArraySequence.tpp"

