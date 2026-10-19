#include "test_file_storage.hpp"
#include "FileStorage.hpp"
#include "base/Sequence.hpp"
#include <iostream>
#include <cassert>
#include <cstdio>

void test_file_storage() {
    std::cout << "=== Тесты FileStorage ===\n";
    
    const std::string testFile = "test_storage.bin";
    
    std::remove(testFile.c_str());
    
    {
        FileStorage storage(testFile);
        Student s1("Иван", "Иванов", 2001, "ИТ-21");
        
        size_t offset = storage.WriteStudent(s1);
        Student r1 = storage.ReadStudent(offset);
        
        assert(r1.firstName == "Иван");
        assert(r1.lastName == "Иванов");
        assert(r1.birthYear == 2001);
        assert(r1.group == "ИТ-21");
        std::cout << "  [OK] Запись и чтение одного студента\n";
    }
    
    {
        FileStorage storage(testFile);
        std::remove(testFile.c_str()); 
        
        Student s1("Иван", "Иванов", 2001, "ИТ-21");
        Student s2("Мария", "Петрова", 2002, "ИТ-22");
        Student s3("Алексей", "Сидоров", 2000, "ИТ-21");
        
        size_t idx1 = storage.WriteStudent(s1);
        size_t idx2 = storage.WriteStudent(s2);
        size_t idx3 = storage.WriteStudent(s3);
        
        Student r1 = storage.ReadStudent(idx1);
        Student r2 = storage.ReadStudent(idx2);
        Student r3 = storage.ReadStudent(idx3);
        
        assert(r1.firstName == s1.firstName && r1.birthYear == s1.birthYear);
        assert(r2.lastName == s2.lastName && r2.group == s2.group);
        assert(r3.birthYear == s3.birthYear && r3.firstName == s3.firstName);
        std::cout << "  [OK] Запись нескольких студентов\n";
    }
    
    {
        FileStorage storage(testFile);
        auto students = storage.ReadAllStudents();
        
        assert(students->GetLength() == 3);
        assert(students->Get(0).firstName == "Иван");
        assert(students->Get(1).firstName == "Мария");
        assert(students->Get(2).firstName == "Алексей");
        delete students;
        std::cout << "  [OK] ReadAllStudents\n";
    }
    
    {
        FileStorage storage(testFile);
        storage.Clear();
        
        auto students = storage.ReadAllStudents();
        assert(students->GetLength() == 0);
        delete students;
        std::cout << "  [OK] Clear\n";
    }
    
    {
        FileStorage storage(testFile);
        storage.Clear();
        
        Student s("", "Тест", 2000, "");
        size_t offset = storage.WriteStudent(s);
        Student r = storage.ReadStudent(offset);
        
        assert(r.firstName.empty());
        assert(r.lastName == "Тест");
        assert(r.group.empty());
        std::cout << "  [OK] Пустые строки\n";
    }
    
    
    {
        FileStorage storage(testFile);
        storage.Clear();
        
        std::string longName(1000, 'A');
        Student s(longName, "Test", 2000, "Group");
        size_t offset = storage.WriteStudent(s);
        Student r = storage.ReadStudent(offset);
        
        assert(r.firstName == longName);
        assert(r.firstName.length() == 1000);
        std::cout << "  [OK] Длинные строки\n";
    }
    
    std::remove(testFile.c_str());
    
    std::cout << "Все тесты FileStorage пройдены!\n\n";
}