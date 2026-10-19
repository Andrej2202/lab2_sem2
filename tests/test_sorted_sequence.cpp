#include "test_sorted_sequence.hpp"
#include "SortedSequence.hpp"
#include "base/Exceptions.hpp"
#include <iostream>
#include <cassert>

void test_sorted_sequence() {
    std::cout << "=== Тесты SortedSequence ===\n";
    
    {
        SortedSequence<int> seq;
        seq.Add(50);
        seq.Add(20);
        seq.Add(80);
        seq.Add(10);
        seq.Add(30);
        
        assert(seq.GetLength() == 5);
        assert(seq.Get(0) == 10);
        assert(seq.Get(1) == 20);
        assert(seq.Get(2) == 30);
        assert(seq.Get(3) == 50);
        assert(seq.Get(4) == 80);
        std::cout << "  [OK] Добавление и сортировка\n";
    }
    
    
    {
        SortedSequence<int> seq;
        seq.Add(10);
        seq.Add(20);
        seq.Add(30);
        seq.Add(40);
        seq.Add(50);
        
        assert(seq.IndexOf(30) == 2);
        assert(seq.IndexOf(10) == 0);
        assert(seq.IndexOf(50) == 4);
        assert(seq.IndexOf(99) == -1);
        std::cout << "  [OK] Бинарный поиск (IndexOf)\n";
    }
    
    
    {
        SortedSequence<int> seq;
        seq.Add(10);
        seq.Add(20);
        seq.Add(30);
        seq.Add(40);
        seq.Add(50);
        
        auto range = seq.GetRange(20, 40);
        assert(range->GetLength() == 3);
        assert(range->Get(0) == 20);
        assert(range->Get(1) == 30);
        assert(range->Get(2) == 40);
        delete range;
        std::cout << "  [OK] Поиск по диапазону\n";
    }
    
    {
        SortedSequence<int> seq;
        seq.Add(10);
        
        bool caught = false;
        try {
            seq.Append(20);
        } catch (const InvalidArgumentException&) {
            caught = true;
        }
        assert(caught);
        
        caught = false;
        try {
            seq.Prepend(5);
        } catch (const InvalidArgumentException&) {
            caught = true;
        }
        assert(caught);
        
        caught = false;
        try {
            seq.InsertAt(15, 1);
        } catch (const InvalidArgumentException&) {
            caught = true;
        }
        assert(caught);
        
        caught = false;
        try {
            seq.Set(0, 99);
        } catch (const InvalidArgumentException&) {
            caught = true;
        }
        assert(caught);
        
        std::cout << "  [OK] Исключения при запрещенных операциях\n";
    }
    

    {
        SortedSequence<int> seq(SortedSequence<int>::DivisionStrategy::GOLDEN_RATIO);
        seq.Add(100);
        seq.Add(50);
        seq.Add(150);
        seq.Add(25);
        seq.Add(75);
        
        assert(seq.GetLength() == 5);
        assert(seq.Get(0) == 25);
        assert(seq.Get(4) == 150);
        std::cout << "  [OK] Стратегия золотого сечения\n";
    }
    
    {
        SortedSequence<int> seq;
        seq.Add(30);
        seq.Add(10);
        seq.Add(20);
        
        int sum = 0;
        for (auto it = seq.begin(); it != seq.end(); ++it) {
            sum += *it;
        }
        assert(sum == 60); 
        std::cout << "  [OK] Итератор\n";
    }
    
    std::cout << "Все тесты SortedSequence пройдены!\n\n";
}