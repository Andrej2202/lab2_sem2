#include "test_index_system.hpp"
#include "IndexSystem.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdio>

void test_index_system() {
    std::cout << "=== Тесты IndexSystem ===\n";
    
    const std::string testDb = "test_index.bin";
    const std::string testCsv = "test_index.csv";
    
    std::remove(testDb.c_str());
    std::remove(testCsv.c_str());
    
    IndexSystem system(testDb);
    
    {
        std::cout << "  [OK] Создание системы\n";
    }
    
    {
        system.AddStudent(Student("Иван", "Иванов", 2001, "ИТ-21"));
        system.AddStudent(Student("Мария", "Петрова", 2002, "ИТ-22"));
        system.AddStudent(Student("Алексей", "Сидоров", 2001, "ИТ-21"));
        
        auto all = system.GetAllStudents();
        assert(all->GetLength() == 3);
        delete all;
        std::cout << "  [OK] Добавление студента\n";
    }
    
    
    {
        auto results = system.SearchByYear(2001);
        assert(results->GetLength() == 2);
        
        bool foundIvan = false;
        bool foundAlexey = false;
        for (size_t i = 0; i < results->GetLength(); i++) {
            if (results->Get(i).firstName == "Иван") foundIvan = true;
            if (results->Get(i).firstName == "Алексей") foundAlexey = true;
        }
        assert(foundIvan && foundAlexey);
        delete results;
        std::cout << "  [OK] Поиск по году\n";
    }
    
    {
        auto results = system.SearchByYearRange(2001, 2002);
        assert(results->GetLength() == 3); 
        delete results;
        std::cout << "  [OK] Поиск по диапазону\n";
    }
    
    {
        auto results = system.SearchByYear(1990);
        assert(results->GetLength() == 0);
        delete results;
        std::cout << "  [OK] Поиск несуществующего года\n";
    }
    
    {
        {
            std::ofstream csv(testCsv);
            csv << "Имя,Фамилия,Год,Группа\n";
            csv << "Петр,Петров,2003,ИТ-23\n";
            csv << "Сергей,Сергеев,2003,ИТ-23\n";
        }
        
        system.LoadFromCSV(testCsv);
        
        auto all = system.GetAllStudents();
        assert(all->GetLength() == 5);
        delete all;
        std::cout << "  [OK] Загрузка из CSV\n";
    }
    
    std::remove(testDb.c_str());
    std::remove(testCsv.c_str());
    
    std::cout << "Все тесты IndexSystem пройдены!\n\n";
}