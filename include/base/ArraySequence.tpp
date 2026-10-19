#pragma region "Constructors"
#include "LinkedList.hpp"
template<class T>
ArraySequence<T>::ArraySequence(T* items, size_t count) {
    data_ = new DynamicArray<T>(items, count);
}

template<class T>
ArraySequence<T>::ArraySequence(const LinkedList <T> & list){
    size_t length = list.GetLength();
    data_ = new DynamicArray<T>(length);
    for(auto i = 0; i < length; i++){
        data_->Set(i, list.Get(i));
    }
}

template<class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> & array){
    data_ = new DynamicArray<T>(*array.data_); 
}

#pragma endregion


#pragma region "InsertMethodes"
template<class T>
void ArraySequence<T>::AppendInternal(T item){
    size_t temp = data_->GetSize();
    data_->Resize(temp + 1);
    data_->Set(temp, item);
}

template<class T>
Sequence<T>* ArraySequence<T>::Append(T item){
    Sequence<T>* target = this->Instance();
    static_cast<ArraySequence<T>*>(target)->AppendInternal(item);
    return target;
}

template<class T>
void ArraySequence<T>::PrependInternal(T item){
    size_t temp = data_->GetSize();
    data_->Resize(temp + 1);
    for(size_t i = temp; i > 0; i--){
        data_->Set(i, data_->Get(i-1));
    }
    data_->Set(0, item);
}

template<class T>
Sequence<T>* ArraySequence<T>::Prepend(T item){
    Sequence<T>* target = this->Instance();
    static_cast<ArraySequence<T>*>(target)->PrependInternal(item);
    return target;
}


template<class T>
void ArraySequence<T>::InsertAtInternal(T item, size_t index){
    size_t temp = data_->GetSize();
    if (index > temp) {
        throw IndexOutOfRangeException("ArraySequence", "InsertAtInternal", "index > size.", temp, index);
    }
    data_->Resize(temp + 1);
    for(size_t i = temp; i > index; i--){
        data_->Set(i, data_->Get(i-1));
    }
    data_->Set(index, item);
}

template<class T>
Sequence<T>* ArraySequence<T>::InsertAt(T item, size_t index){
    Sequence<T>* target = this->Instance();
    static_cast<ArraySequence<T>*>(target)->InsertAtInternal(item, index);
    return target;
}


template<class T>
void ArraySequence<T>::ConcatInternal(Sequence<T>* smth) {
    if (smth == nullptr) {
        throw InvalidArgumentException("ArraySequence", "concatenatination", "Cannot concatenate with null sequence.");
    }

    size_t currentSize = data_->GetSize();
    size_t otherSize = smth->GetLength();
    if (otherSize == 0) return;

    data_->Resize(currentSize + otherSize);
    for (size_t i = 0; i < otherSize; ++i) {
        data_->Set(currentSize + i, smth->Get(i));
    }
}

template<class T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* smth){
    Sequence<T>* target = this->Clone();
    static_cast<ArraySequence<T>*>(target)->ConcatInternal(smth);
    return target;
}

#pragma endregion
