#pragma once
#include "ListSequence.hpp"

template<class T>
class MutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;

    ListSequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
    
    Sequence<T>* Instance() override { 
        return this;
    }

    Sequence<T>* CreateEmpty() const override { 
        return new MutableListSequence<T>(); 
    }

    Sequence<T>*  GetSubsequence(size_t startIndex, size_t endIndex) const override;


    void Set(size_t index, T item) override {
        if (index > this->GetLength()) {
            throw IndexOutOfRangeException("ArraySequence", "InsertAtInternal", "index > size.", this->GetLength(), index);
        }
        this->data_->Set(index, item);
    }
    
};


template<class T>
Sequence<T>*  MutableListSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex > endIndex)
        throw IndexOutOfRangeException("ListSequence", "GetSubsequence", "Invalid subsequence range.", endIndex, startIndex);
    LinkedList<T> subList = this->data_->GetSubList(startIndex, endIndex);
    
    return new MutableListSequence<T>(std::move(subList));
}
