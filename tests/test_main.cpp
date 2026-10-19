#include "Student.hpp"
#include "Pair.hpp"
#include <iostream>
#include <cassert>

void test_student_creation() {
    std::cout << "Тест 1: Создание студента... ";
    Student s("Тест", "Тестов", 2000, "ТГ-01");
    assert(s.firstName == "Тест");
    assert(s.lastName == "Тестов");
    assert(s.birthYear == 2000);
    assert(s.group == "ТГ-01");
    std::cout << "ПРОЙДЕН\n";
}

void test_pair_creation() {
    std::cout << "Тест 2: Создание Pair... ";
    Pair<int, Student> p(2000, Student("Иван", "Иванов", 2000, "ИТ-21"));
    assert(p.first == 2000);
    assert(p.second.firstName == "Иван");
    std::cout << "ПРОЙДЕН\n";
}

void test_pair_comparison() {
    std::cout << "Тест 3: Сравнение Pair... ";
    Pair<int, Student> p1(2000, Student());
    Pair<int, Student> p2(2001, Student());
    Pair<int, Student> p3(2000, Student());
    
    assert(p1 < p2);
    assert(p2 > p1);
    assert(p1 == p3);
    assert(p1 != p2);
    assert(p1 <= p3);
    assert(p1 >= p3);
    std::cout << "ПРОЙДЕН\n";
}

int main() {
    std::cout << "=== Запуск тестов ===\n\n";
    
    test_student_creation();
    test_pair_creation();
    test_pair_comparison();
    
    std::cout << "\n=== Все тесты пройдены! ===\n";
    
    return 0;
}