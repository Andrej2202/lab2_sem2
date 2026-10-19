#pragma once

#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"
#include "base/Exceptions.hpp"
#include <vector>

template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };

    Node* root_;
    size_t size_;

    void InsertRecursive(Node*& node, const T& value);
    void DestroyRecursive(Node* node);
    void GetRangeRecursive(Node* node, const T& min, const T& max, MutableArraySequence<T>* result) const;

public:
    BinaryTree();
    ~BinaryTree();

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    void Insert(const T& value);
    size_t GetSize() const;
    bool IsEmpty() const;

    Sequence<T>* GetRange(const T& min, const T& max) const;

    class Iterator {
    private:
        std::vector<Node*> stack_; 

        void pushLeftBranch(Node* node);

    public:
        Iterator();
        Iterator(Node* root);

        T operator*() const;
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
    };

    Iterator begin() const;
    Iterator end() const;
};

#include "BinaryTree.tpp"