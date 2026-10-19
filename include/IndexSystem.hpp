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
#include <algorithm>
#include <limits>



class IndexSystem {
private:
    FileStorage* storage_;
    BinaryTree<Pair<int, size_t>>* treeIndex_;
    SortedSequence<Pair<int, size_t>>* sortedIndex_;

    Sequence<Student>* FetchStudents(Sequence<Pair<int, size_t>>* indexResult) const;

public:
    IndexSystem(const std::string& dataFilePath);
    ~IndexSystem();

    // Загрузка из CSV (std::string версия - для тестов)
    void LoadFromCSV(const std::string& csvPath);
    //void LoadFromCSV(const QString& csvPath);
    
    
    void AddStudent(const Student& student);
    Sequence<Student>* SearchByYear(int year) const;
    Sequence<Student>* SearchByYearRange(int minYear, int maxYear) const;
    Sequence<Student>* GetAllStudents() const;

    struct BenchmarkResult {
        std::string dataType;
        size_t dataSize;
        double treeBuildTime_ms;
        double sortedBuildTime_ms;
        double treeSearchTime_ms;
        double sortedSearchTime_ms;
    };

    BenchmarkResult RunBenchmark(size_t N, bool isSorted, bool isAscending) const;
    void RunFullBenchmark(const std::string& outputFile) const;
};