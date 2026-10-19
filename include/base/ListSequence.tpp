#pragma region "GetMethode"
template<class T>
T ListSequence<T>::GetFirst() const { return data_->GetFirst(); }
template<class T>
T ListSequence<T>::GetLast() const { return data_->GetLast(); }
template<class T>
T ListSequence<T>::Get(size_t index) const { return data_->Get(index); }
template<class T>
size_t ListSequence<T>::GetLength() const { return data_->GetLength(); }

#pragma endregion

#pragma region "InsertMethode"
template<class T>
void ListSequence<T>::AppendInternal(T item) {
    data_->Append(item);
}

template<class T>
Sequence<T>* ListSequence<T>::Append(T item){
    Sequence<T>* target = this->Instance();
    static_cast<ListSequence<T>*>(target)->AppendInternal(item);
    return target;
}

template<class T>
void ListSequence<T>::PrependInternal(T item) {
    data_->Prepend(item);
}

template<class T>
Sequence<T>* ListSequence<T>::Prepend(T item){
    Sequence<T>* target = this->Instance();
    static_cast<ListSequence<T>*>(target)->PrependInternal(item);
    return target;
}

template<class T>
void ListSequence<T>::InsertAtInternal(T item, size_t index) {
    data_->InsertAt(item, index);
}

template<class T>
Sequence<T>* ListSequence<T>::InsertAt(T item, size_t index){
    Sequence<T>* target = this->Instance();
    static_cast<ListSequence<T>*>(target)->InsertAtInternal(item, index);
    return target;
}

template<class T>
void ListSequence<T>::ConcatInternal(Sequence<T>* smth) {
    if (smth == nullptr) return;
    size_t len = smth->GetLength();
    for (size_t i = 0; i < len; ++i) {
        data_->Append(smth->Get(i));
    }
}

template<class T>
Sequence<T>* ListSequence<T>::Concat(Sequence<T>* smth){
    Sequence<T>* target = this->Clone();
    static_cast<ListSequence<T>*>(target)->ConcatInternal(smth);
    return target;
}
#pragma endregion