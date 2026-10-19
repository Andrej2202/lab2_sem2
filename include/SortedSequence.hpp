#pragma once

#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"
#include "base/Exceptions.hpp"
#include <cmath>

template <typename T>
class SortedSequence : public Sequence<T> {
public:
    enum class DivisionStrategy {
        HALF,
        GOLDEN_RATIO
    };

private:
    MutableArraySequence<T>* data_;
    DivisionStrategy strategy_;

    size_t BinarySearch(const T& item) const;
    size_t FindInsertPosition(const T& item) const;

public:
    SortedSequence(DivisionStrategy strategy = DivisionStrategy::HALF);
    SortedSequence(const SortedSequence<T>& other);
    ~SortedSequence() override;

    SortedSequence<T>& operator=(const SortedSequence<T>& other);

    void Add(const T& item);
    int IndexOf(const T& item) const;
    Sequence<T>* GetRange(const T& min, const T& max) const;
    void SetDivisionStrategy(DivisionStrategy strategy);

    T GetFirst() const override;
    T GetLast() const override;
    T Get(size_t index) const override;
    size_t GetLength() const override;
    
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    
    
    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, size_t index) override;
    Sequence<T>* Concat(Sequence<T>* list) override;
    void Set(size_t index, T data) override;
    
    Sequence<T>* CreateEmpty() const override;
    SequenceIterator<T> begin() const override;
    SequenceIterator<T> end() const override;
};

#include "SortedSequence.tpp"