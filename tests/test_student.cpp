#include "test_student.hpp"
#include "Student.hpp"
#include <iostream>
#include <cassert>

void test_student() {
    std::cout << "=== Тесты Student ===\n";
    {
        Student s;
        assert(s.firstName.empty());
        assert(s.lastName.empty());
        assert(s.birthYear == 0);
        assert(s.group.empty());
        std::cout << "  [OK] Конструктор по умолчанию\n";
    }
    
    {
        Student s("Иван", "Иванов", 2001, "ИТ-21");
        assert(s.firstName == "Иван");
        assert(s.lastName == "Иванов");
        assert(s.birthYear == 2001);
        assert(s.group == "ИТ-21");
        std::cout << "  [OK] Конструктор с параметрами\n";
    }
    
    {
        Student s("Тест", "Тестов", 2000, "ТГ-01");
        s.print(); 
        std::cout << "  [OK] Метод print\n";
    }
    
    std::cout << "Все тесты Student пройдены!\n\n";
}