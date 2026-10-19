template <typename T>
BinaryTree<T>::BinaryTree() : root_(nullptr), size_(0) {}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    DestroyRecursive(root_);
}

template <typename T>
void BinaryTree<T>::DestroyRecursive(Node* node) {
    if (node != nullptr) {
        DestroyRecursive(node->left);
        DestroyRecursive(node->right);
        delete node;
    }
}

template <typename T>
void BinaryTree<T>::InsertRecursive(Node*& node, const T& value) {
    if (node == nullptr) {
        node = new Node(value);
        size_++;
    } else if (value < node->data) {
        InsertRecursive(node->left, value);
    } else {
        InsertRecursive(node->right, value);
    }
}

template <typename T>
void BinaryTree<T>::Insert(const T& value) {
    InsertRecursive(root_, value);
}

template <typename T>
size_t BinaryTree<T>::GetSize() const {
    return size_;
}

template <typename T>
bool BinaryTree<T>::IsEmpty() const {
    return size_ == 0;
}

template <typename T>
Sequence<T>* BinaryTree<T>::GetRange(const T& min, const T& max) const {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    GetRangeRecursive(root_, min, max, result);
    return result;
}

template <typename T>
void BinaryTree<T>::GetRangeRecursive(Node* node, const T& min, const T& max, MutableArraySequence<T>* result) const {
    if (node == nullptr) return;

    if (node->data > min) {
        GetRangeRecursive(node->left, min, max, result);
    }

    if (node->data >= min && node->data <= max) {
        result->Append(node->data);
    }

    if (node->data < max) {
        GetRangeRecursive(node->right, min, max, result);
    }
}

template <typename T>
void BinaryTree<T>::Iterator::pushLeftBranch(Node* node) {
    while (node != nullptr) {
        stack_.push_back(node);
        node = node->left;
    }
}

template <typename T>
BinaryTree<T>::Iterator::Iterator() {}

template <typename T>
BinaryTree<T>::Iterator::Iterator(Node* root) {
    pushLeftBranch(root);
}

template <typename T>
T BinaryTree<T>::Iterator::operator*() const {
    if (stack_.empty()) {
        throw InvalidArgumentException("BinaryTree::Iterator", "operator*", "Dereferencing end iterator");
    }
    return stack_.back()->data;
}

template <typename T>
typename BinaryTree<T>::Iterator& BinaryTree<T>::Iterator::operator++() {
    if (stack_.empty()) {
        throw InvalidArgumentException("BinaryTree::Iterator", "operator++", "Incrementing end iterator");
    }
    
    Node* current = stack_.back();
    stack_.pop_back();

    if (current->right != nullptr) {
        pushLeftBranch(current->right);
    }
    
    return *this;
}

template <typename T>
bool BinaryTree<T>::Iterator::operator!=(const Iterator& other) const {
    return stack_ != other.stack_;
}

template <typename T>
bool BinaryTree<T>::Iterator::operator==(const Iterator& other) const {
    return stack_ == other.stack_;
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::begin() const {
    return Iterator(root_);
}

template <typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::end() const {
    return Iterator();
}