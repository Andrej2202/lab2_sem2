#include "test_pair.hpp"
#include "Pair.hpp"
#include <iostream>
#include <cassert>

void test_pair() {
    std::cout << "=== Тесты Pair ===\n";
    
    {
        Pair<int, int> p;
        assert(p.first == 0);
        assert(p.second == 0);
        std::cout << "  [OK] Конструктор по умолчанию\n";
    }
    
    {
        Pair<int, std::string> p(42, "hello");
        assert(p.first == 42);
        assert(p.second == "hello");
        std::cout << "  [OK] Конструктор с параметрами\n";
    }
    
    {
        Pair<int, int> p1(10, 1);
        Pair<int, int> p2(20, 2);
        Pair<int, int> p3(10, 3);
        
        assert(p1 < p2);
        assert(p2 > p1);
        assert(p1 == p3); 
        assert(p1 != p2);
        assert(p1 <= p3);
        assert(p1 >= p3);
        std::cout << "  [OK] Операторы сравнения\n";
    }
    
    
    {
        Pair<std::string, double> p("pi", 3.14);
        assert(p.first == "pi");
        assert(p.second > 3.0);
        std::cout << "  [OK] Pair с разными типами\n";
    }
    
    std::cout << "Все тесты Pair пройдены!\n\n";
}