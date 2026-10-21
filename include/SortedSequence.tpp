template <typename T>
SortedSequence<T>::SortedSequence(DivisionStrategy strategy) 
    : data_(new MutableArraySequence<T>()), strategy_(strategy) {}

template <typename T>
SortedSequence<T>::SortedSequence(const SortedSequence<T>& other) 
    : data_(new MutableArraySequence<T>(*other.data_)), strategy_(other.strategy_) {}

template <typename T>
SortedSequence<T>::~SortedSequence() {
    delete data_;
}

template <typename T>
SortedSequence<T>& SortedSequence<T>::operator=(const SortedSequence<T>& other) {
    if (this != &other) {
        delete data_;
        data_ = new MutableArraySequence<T>(*other.data_);
        strategy_ = other.strategy_;
    }
    return *this;
}


template <typename T>
void SortedSequence<T>::SetDivisionStrategy(DivisionStrategy strategy) {
    strategy_ = strategy;
}

template <typename T>
size_t SortedSequence<T>::BinarySearch(const T& item) const {
    size_t left = 0;
    size_t right = data_->GetLength();
    
    while (left < right) {
        size_t mid;
        if (strategy_ == DivisionStrategy::HALF) {
            mid = left + (right - left) / 2;
        } else {
            mid = left + static_cast<size_t>((right - left) * 0.382);
        }

        T midVal = data_->Get(mid);
        if (midVal < item) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

template <typename T>
size_t SortedSequence<T>::FindInsertPosition(const T& item) const {
    size_t left = 0;
    size_t right = data_->GetLength();
    if (right == 0) return 0;
    right--;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        T midVal = data_->Get(mid);
        if (midVal < item) {
            left = mid + 1;
        } else {
            if (mid == 0) return 0;
            right = mid - 1;
        }
    }
    return left;
}

template <typename T>
void SortedSequence<T>::Add(const T& item) {
    size_t pos = FindInsertPosition(item);
    data_->InsertAt(item, pos); 
}

template <typename T>
int SortedSequence<T>::IndexOf(const T& item) const {
    size_t index = BinarySearch(item);
    if (index == SIZE_MAX) return -1;
    return static_cast<int>(index);
}

template <typename T>
Sequence<T>* SortedSequence<T>::GetRange(const T& min, const T& max) const {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    size_t len = data_->GetLength();
    
    size_t start = BinarySearch(min);
    
    for (size_t i = start; i < len; i++) {
        T val = data_->Get(i);
        if (val > max) break;
        result->Append(val);
    }
    
    return result;
}

template <typename T>
T SortedSequence<T>::GetFirst() const { return data_->GetFirst(); }

template <typename T>
T SortedSequence<T>::GetLast() const { return data_->GetLast(); }

template <typename T>
T SortedSequence<T>::Get(size_t index) const { return data_->Get(index); }

template <typename T>
size_t SortedSequence<T>::GetLength() const { return data_->GetLength(); }

template <typename T>
Sequence<T>* SortedSequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    return data_->GetSubsequence(startIndex, endIndex);
}

template <typename T>
Sequence<T>* SortedSequence<T>::Append(T item) {
    (void)item;
    throw InvalidArgumentException("SortedSequence", "Append", "Нельзя использовать Append. Используйте Add()");
}

template <typename T>
Sequence<T>* SortedSequence<T>::Prepend(T item) {
    (void)item;
    throw InvalidArgumentException("SortedSequence", "Prepend", "Нельзя использовать Prepend. Используйте Add()");
}

template <typename T>
Sequence<T>* SortedSequence<T>::InsertAt(T item, size_t index) {
    (void)item;
    (void)index;
    throw InvalidArgumentException("SortedSequence", "InsertAt", "Нельзя использовать InsertAt. Используйте Add()");
}

template <typename T>
Sequence<T>* SortedSequence<T>::Concat(Sequence<T>* list) {
    (void)list;
    throw InvalidArgumentException("SortedSequence", "Concat", "Нельзя использовать Concat. Добавляйте элементы через Add()");
}

template <typename T>
void SortedSequence<T>::Set(size_t index, T data) {
    (void)index;
    (void)data;
    throw InvalidArgumentException("SortedSequence", "Set", "Нельзя изменять элементы по индексу. Это нарушит сортировку");
}

template <typename T>
Sequence<T>* SortedSequence<T>::CreateEmpty() const {
    return new SortedSequence<T>(strategy_);
}

template <typename T>
SequenceIterator<T> SortedSequence<T>::begin() const {
    return data_->begin();
}

template <typename T>
SequenceIterator<T> SortedSequence<T>::end() const {
    return data_->end();
}