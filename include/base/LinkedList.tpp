#pragma region "Constructors"
template <class T>
LinkedList<T>::LinkedList(T* items, size_t count) : head_(nullptr), tail_(nullptr) {
    for (size_t i = 0; i < count; i++){
        Append(items[i]);
    }
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head_(nullptr), tail_(nullptr) {
    Node_* current = other.head_;
    while (current != nullptr) {
        Node_* newNode_ = new Node_(current->data);
        if (!head_) head_ = tail_ = newNode_;
        else {
            tail_->next = newNode_;
            tail_ = newNode_;
        }
        current = current->next;
    }
}

#pragma endregion

#pragma region "GetMethodes"
template<class T>
T LinkedList<T>::GetFirst() const {
    if (head_ == nullptr) 
        throw IndexOutOfRangeException("LinkedList", "GetFirst", "LinkedList is empty");
    return head_->data;
}

template<class T>
T LinkedList<T>::GetLast() const {
    if (head_ == nullptr) 
        throw IndexOutOfRangeException("LinkedList", "GetLast", "LinkedList is empty");
    return tail_->data;
}

template<class T>
T LinkedList<T>::Get(size_t index) const {

    Node_* current = head_;
    size_t i;
    for (i = 0; i < index && current != nullptr; ++i) {
        current = current->next;
    }
    if (current == nullptr) {
        throw IndexOutOfRangeException("LinkedList", "Get", "Index > length", i, index);
    }

    return current->data;
}

template<class T>
void LinkedList<T>::Set(size_t index, T data) const {

    Node_* current = head_;
    size_t i;
    for (i = 0; i < index && current != nullptr; ++i) {
        current = current->next;
    }
    if (current == nullptr) {
        throw IndexOutOfRangeException("LinkedList", "Get", "Index > length", i, index);
    }

    current->data = data;
}

template<class T>
size_t LinkedList<T>::GetLength() const {
    size_t count = 0;
    Node_* current = head_;
    while (current != nullptr) {
        ++count;
        current = current->next;
    }
    return count;
}

template<class T>
LinkedList<T> LinkedList<T>::GetSubList(size_t startIndex, size_t endIndex) const {
    size_t len = GetLength();
    if (startIndex >= len || endIndex >= len) {
        throw IndexOutOfRangeException("LinkedList", "GetSubList", "Invalid sublist range", std::max(startIndex, endIndex), len);
    }
    if (startIndex > endIndex) {
        throw IndexOutOfRangeException("LinkedList", "GetSubList", "Invalid sublist range", endIndex, startIndex);
    }

    LinkedList<T> result;
    Node_* current = head_;

    for (size_t i = 0; i < startIndex; ++i) {
        current = current->next;
    }
    size_t count = endIndex - startIndex + 1;
    for (size_t i = 0; i < count; ++i) {
        result.Append(current->data);
        current = current->next;
    }

    return result;
}

#pragma endregion

#pragma region "InsertMethodes"
template<class T>
void LinkedList<T>::Append(T item) {
    Node_* newNode = new Node_(item);
    if (head_ == nullptr) {
        head_ = tail_ = newNode;
    } else {
        tail_->next = newNode;
        tail_ = newNode;
    }
}

template<class T>
void LinkedList<T>::Prepend(T item) {
    Node_* newNode = new Node_(item);
    if (head_ == nullptr) {
        head_ = tail_ = newNode;
    } else {
        newNode->next = head_;
        head_ = newNode;
    }
}

template<class T>
auto LinkedList<T>::InsertAt(T item, size_t index) -> void {
    
    size_t currentLength = GetLength();
    if (index > currentLength) 
        throw IndexOutOfRangeException("LinkedList", "InsertAt", "index > length", currentLength, index);
        
    if (index == 0) {
        Prepend(item);
        return;
    }

    Node_* current = head_;
    for (size_t i = 0; i < index - 1; ++i) {
        current = current->next;
    }
    Node_* newNode = new Node_(item);
    newNode->next = current->next;
    current->next = newNode;
}

template<class T>
auto LinkedList<T>::Concat(LinkedList<T>* list) -> LinkedList<T>* {
    if (list == nullptr) 
        return new LinkedList<T>(*this);
    
    LinkedList<T>* result = new LinkedList<T>(*this);
    Node_* current = list->head_;

    while (current != nullptr) {
        result->Append(current->data);
        current = current->next;
    }
    return result;
}
#pragma endregion