#include "test_binary_tree.hpp"
#include "BinaryTree.hpp"
#include <iostream>
#include <cassert>

void test_binary_tree() {
    std::cout << "=== Тесты BinaryTree ===\n";
    
    {
        BinaryTree<int> tree;
        assert(tree.IsEmpty());
        assert(tree.GetSize() == 0);
        
        tree.Insert(50);
        tree.Insert(30);
        tree.Insert(70);
        
        assert(tree.GetSize() == 3);
        assert(!tree.IsEmpty());
        std::cout << "  [OK] Вставка и размер\n";
    }
    
    {
        BinaryTree<int> tree;
        tree.Insert(50);
        tree.Insert(30);
        tree.Insert(70);
        tree.Insert(20);
        tree.Insert(40);
        tree.Insert(60);
        tree.Insert(80);
        
        int expected[] = {20, 30, 40, 50, 60, 70, 80};
        int i = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            assert(*it == expected[i]);
            i++;
        }
        assert(i == 7);
        std::cout << "  [OK] In-order обход (итератор)\n";
    }
    
    {
        BinaryTree<int> tree;
        tree.Insert(50);
        tree.Insert(30);
        tree.Insert(70);
        tree.Insert(20);
        tree.Insert(40);
        tree.Insert(60);
        tree.Insert(80);
        
        auto range = tree.GetRange(30, 60);
        assert(range->GetLength() == 4); // 30, 40, 50, 60
        assert(range->Get(0) == 30);
        assert(range->Get(1) == 40);
        assert(range->Get(2) == 50);
        assert(range->Get(3) == 60);
        delete range;
        std::cout << "  [OK] Поиск по диапазону\n";
    }
    
    {
        BinaryTree<int> tree;
        tree.Insert(10);
        tree.Insert(20);
        tree.Insert(30);
        
        auto range = tree.GetRange(50, 100);
        assert(range->GetLength() == 0);
        delete range;
        std::cout << "  [OK] Пустой диапазон\n";
    }
    
    {
        BinaryTree<int> tree;
        tree.Insert(10);
        tree.Insert(10);
        tree.Insert(10);
        
        assert(tree.GetSize() == 3);
        
        int count = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            assert(*it == 10);
            count++;
        }
        assert(count == 3);
        std::cout << "  [OK] Дубликаты\n";
    }
    
    {
        BinaryTree<int> tree;
        tree.Insert(1);
        tree.Insert(2);
        tree.Insert(3);
        tree.Insert(4);
        tree.Insert(5);
        
        int expected[] = {1, 2, 3, 4, 5};
        int i = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            assert(*it == expected[i]);
            i++;
        }
        std::cout << "  [OK] Вырожденное дерево (отсортированные данные)\n";
    }
    
    std::cout << "Все тесты BinaryTree пройдены!\n\n";
}