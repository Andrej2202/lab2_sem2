#pragma once

#include "Student.hpp"
#include "Pair.hpp"
#include "BinaryTree.hpp"
#include "SortedSequence.hpp"
#include "FileStorage.hpp"
#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>
#include <random>

class IndexSystem {
private:
    FileStorage* storage_;
    BinaryTree<Pair<int, size_t>>* treeIndex_;
    SortedSequence<Pair<int, size_t>>* sortedIndex_;

    // Вспомогательный метод: читает студентов по смещениям из индекса
    Sequence<Student>* FetchStudents(Sequence<Pair<int, size_t>>* indexResult) const;

public:
    IndexSystem(const std::string& dataFilePath);
    ~IndexSystem();

    // Загрузка из CSV (формат: Имя,Фамилия,Год,Группа)
    void LoadFromCSV(const std::string& csvPath);
    
    // Добавление одного студента
    void AddStudent(const Student& student);

    // Точный поиск по году
    Sequence<Student>* SearchByYear(int year) const;
    
    // Поиск по диапазону [minYear, maxYear]
    Sequence<Student>* SearchByYearRange(int minYear, int maxYear) const;
    
    // Получить всех студентов (для UI)
    Sequence<Student>* GetAllStudents() const;

    // --- Бенчмарк ---
    struct BenchmarkResult {
        std::string dataType; // "Sorted Asc", "Sorted Desc", "Random"
        size_t dataSize;
        double treeBuildTime_ms;
        double sortedBuildTime_ms;
        double treeSearchTime_ms;
        double sortedSearchTime_ms;
    };

    // Генерация тестовых данных и замер времени
    BenchmarkResult RunBenchmark(size_t N, bool isSorted, bool isAscending) const;
    
    // Запуск полного бенчмарка по всем сценариям из ТЗ
    void RunFullBenchmark(const std::string& outputFile) const;
};