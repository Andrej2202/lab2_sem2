#include "Student.hpp"
#include "Pair.hpp"
#include "SortedSequence.hpp"
#include "BinaryTree.hpp"
#include "FileStorage.hpp"
#include <iostream>
#include <cassert>


void test_file_storage() {
    std::cout << "Тест 6: FileStorage (Бинарная сериализация)... " << std::flush;
    
    FileStorage storage("test_students.bin");
    storage.Clear(); // Очищаем файл перед тестом

    // Создаем студентов
    Student s1("Иван", "Иванов", 2001, "ИТ-21");
    Student s2("Мария", "Петрова", 2002, "ИТ-22");
    Student s3("Алексей", "Сидоров", 2000, "ИТ-21");

    // Записываем и запоминаем индексы (смещения)
    size_t idx1 = storage.WriteStudent(s1);
    size_t idx2 = storage.WriteStudent(s2);
    size_t idx3 = storage.WriteStudent(s3);

    // Читаем обратно по индексам
    Student r1 = storage.ReadStudent(idx1);
    Student r2 = storage.ReadStudent(idx2);
    Student r3 = storage.ReadStudent(idx3);

    // Проверяем, что данные совпадают
    assert(r1.firstName == s1.firstName && r1.birthYear == s1.birthYear);
    assert(r2.lastName == s2.lastName && r2.group == s2.group);
    assert(r3.birthYear == s3.birthYear);

    // Проверяем чтение всех студентов
    Sequence<Student>* all = storage.ReadAllStudents();
    assert(all->GetLength() == 3);
    assert(all->Get(1).firstName == "Мария");
    delete all;

    std::cout << "ПРОЙДЕН\n";
}


void test_student_creation() {
    std::cout << "Тест 1: Создание студента... " << std::flush;
    Student s("Тест", "Тестов", 2000, "ТГ-01");
    assert(s.firstName == "Тест");
    assert(s.lastName == "Тестов");
    assert(s.birthYear == 2000);
    assert(s.group == "ТГ-01");
    std::cout << "ПРОЙДЕН\n";
}

void test_binary_tree() {
    std::cout << "Тест 5: BinaryTree и Итератор... " << std::flush;
    BinaryTree<int> tree;
    
    tree.Insert(50);
    tree.Insert(20);
    tree.Insert(80);
    tree.Insert(10);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(90);

    assert(tree.GetSize() == 7);

    int expected[] = {10, 20, 30, 50, 70, 80, 90};
    int i = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        assert(*it == expected[i]);
        i++;
    }
    assert(i == 7);

    Sequence<int>* range = tree.GetRange(25, 75);
    assert(range->GetLength() == 3); 
    assert(range->Get(0) == 30);
    assert(range->Get(1) == 50);
    assert(range->Get(2) == 70);
    delete range;

    std::cout << "ПРОЙДЕН\n";
}

void test_pair_creation() {
    std::cout << "Тест 2: Создание Pair... " << std::flush;
    Pair<int, Student> p(2000, Student("Иван", "Иванов", 2000, "ИТ-21"));
    assert(p.first == 2000);
    assert(p.second.firstName == "Иван");
    std::cout << "ПРОЙДЕН\n";
}

void test_pair_comparison() {
    std::cout << "Тест 3: Сравнение Pair... " << std::flush;
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

void test_sorted_sequence() {
    std::cout << "Тест 4: SortedSequence (Half)... " << std::flush;
    SortedSequence<int> seq(SortedSequence<int>::DivisionStrategy::HALF);
    
    seq.Add(50);
    seq.Add(20);
    seq.Add(80);
    seq.Add(10);
    seq.Add(30);

    assert(seq.GetLength() == 5);
    assert(seq.Get(0) == 10);
    assert(seq.Get(4) == 80);

    assert(seq.IndexOf(30) == 2);
    assert(seq.IndexOf(99) == -1);

    Sequence<int>* range = seq.GetRange(20, 50);
    assert(range->GetLength() == 3);
    assert(range->Get(0) == 20);
    assert(range->Get(2) == 50);
    delete range;

    bool exceptionThrown = false;
    try {
        seq.Append(100);
    } catch (const InvalidArgumentException&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "ПРОЙДЕН\n";
}

int main() {
    std::cout << "=== Запуск тестов ===\n\n";
    
    test_student_creation();
    test_pair_creation();
    test_pair_comparison();
    test_sorted_sequence();
    test_binary_tree(); 
    test_file_storage();
    
    std::cout << "\n=== Все тесты пройдены! ===\n";
    
    return 0;
}