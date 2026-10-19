#include "IndexSystem.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <limits>
#include <cstdio>

// ============================================
// КОНСТРУКТОР И ДЕСТРУКТОР
// ============================================

IndexSystem::IndexSystem(const std::string& dataFilePath) {
    storage_ = new FileStorage(dataFilePath);
    treeIndex_ = new BinaryTree<Pair<int, size_t>>();
    sortedIndex_ = new SortedSequence<Pair<int, size_t>>();
}

IndexSystem::~IndexSystem() {
    delete storage_;
    delete treeIndex_;
    delete sortedIndex_;
}

// ============================================
// ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ
// ============================================

Sequence<Student>* IndexSystem::FetchStudents(Sequence<Pair<int, size_t>>* indexResult) const {
    MutableArraySequence<Student>* result = new MutableArraySequence<Student>();
    if (!indexResult) return result;
    
    size_t len = indexResult->GetLength();
    for (size_t i = 0; i < len; i++) {
        Pair<int, size_t> p = indexResult->Get(i);
        try {
            result->Append(storage_->ReadStudent(p.second));
        } catch (const std::exception& e) {
            std::cerr << "Ошибка чтения студента по смещению " << p.second << ": " << e.what() << "\n";
        }
    }
    delete indexResult;
    return result;
}

// ============================================
// ЗАГРУЗКА ИЗ CSV (std::string версия — для тестов и консоли)
// ============================================

void IndexSystem::LoadFromCSV(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << csvPath << "\n";
        return;
    }

    std::string line;
    bool headerSkipped = false;
    int count = 0;
    int skipped = 0;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Пропускаем заголовок
        if (!headerSkipped) {
            if (line.find("Год") != std::string::npos || line.find("Year") != std::string::npos ||
                line.find("Имя") != std::string::npos || line.find("Name") != std::string::npos) {
                headerSkipped = true;
                continue;
            }
            headerSkipped = true;
        }
        
        std::stringstream ss(line);
        std::string firstName, lastName, yearStr, group;
        
        try {
            if (std::getline(ss, firstName, ',') &&
                std::getline(ss, lastName, ',') &&
                std::getline(ss, yearStr, ',') &&
                std::getline(ss, group)) {
                
                // Убираем пробелы и символы новой строки
                while (!yearStr.empty() && (yearStr.back() == '\r' || yearStr.back() == '\n' || yearStr.back() == ' ')) {
                    yearStr.pop_back();
                }
                while (!group.empty() && (group.back() == '\r' || group.back() == '\n' || group.back() == ' ')) {
                    group.pop_back();
                }
                while (!firstName.empty() && (firstName.back() == '\r' || firstName.back() == '\n' || firstName.back() == ' ')) {
                    firstName.pop_back();
                }
                while (!lastName.empty() && (lastName.back() == '\r' || lastName.back() == '\n' || lastName.back() == ' ')) {
                    lastName.pop_back();
                }
                
                int year = std::stoi(yearStr);
                Student student(firstName, lastName, year, group);
                AddStudent(student);
                count++;
            } else {
                skipped++;
            }
        } catch (const std::exception& e) {
            skipped++;
        }
    }
    
    std::cout << "Загружено " << count << " записей из CSV";
    if (skipped > 0) {
        std::cout << " (пропущено " << skipped << " некорректных строк)";
    }
    std::cout << ".\n";
}

// ============================================
// ДОБАВЛЕНИЕ СТУДЕНТА
// ============================================

void IndexSystem::AddStudent(const Student& student) {
    try {
        size_t offset = storage_->WriteStudent(student);
        Pair<int, size_t> record(student.birthYear, offset);
        treeIndex_->Insert(record);
        sortedIndex_->Add(record);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка добавления студента: " << e.what() << "\n";
        throw;
    }
}

// ============================================
// ПОИСК
// ============================================

Sequence<Student>* IndexSystem::SearchByYear(int year) const {
    try {
        auto range = sortedIndex_->GetRange(
            Pair<int, size_t>(year, 0), 
            Pair<int, size_t>(year, std::numeric_limits<size_t>::max())
        );
        return FetchStudents(range);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка поиска: " << e.what() << "\n";
        return new MutableArraySequence<Student>();
    } catch (...) {
        std::cerr << "Неизвестная ошибка поиска\n";
        return new MutableArraySequence<Student>();
    }
}

Sequence<Student>* IndexSystem::SearchByYearRange(int minYear, int maxYear) const {
    try {
        auto range = treeIndex_->GetRange(
            Pair<int, size_t>(minYear, 0), 
            Pair<int, size_t>(maxYear, std::numeric_limits<size_t>::max())
        );
        return FetchStudents(range);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка поиска по диапазону: " << e.what() << "\n";
        return new MutableArraySequence<Student>();
    } catch (...) {
        std::cerr << "Неизвестная ошибка поиска по диапазону\n";
        return new MutableArraySequence<Student>();
    }
}

Sequence<Student>* IndexSystem::GetAllStudents() const {
    try {
        return storage_->ReadAllStudents();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка чтения всех студентов: " << e.what() << "\n";
        return new MutableArraySequence<Student>();
    } catch (...) {
        std::cerr << "Неизвестная ошибка чтения всех студентов\n";
        return new MutableArraySequence<Student>();
    }
}

// ============================================
// БЕНЧМАРК
// ============================================

IndexSystem::BenchmarkResult IndexSystem::RunBenchmark(size_t N, bool isSorted, bool isAscending) const {
    BenchmarkResult result;
    result.dataSize = N;
    result.dataType = isSorted ? (isAscending ? "Sorted Asc" : "Sorted Desc") : "Random";

    // 1. Генерация тестовых данных
    std::vector<Student> students;
    students.reserve(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> yearDist(1990, 2010);
    
    for (size_t i = 0; i < N; i++) {
        students.push_back(Student("Name", "LastName", yearDist(gen), "Group"));
    }

    // Сортировка если нужно
    if (isSorted) {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.birthYear < b.birthYear;
        });
        if (!isAscending) {
            std::reverse(students.begin(), students.end());
        }
    }

    // 2. Создаём временные структуры для бенчмарка
    FileStorage tempStorage("benchmark_temp.bin");
    tempStorage.Clear();
    BinaryTree<Pair<int, size_t>> tempTree;
    SortedSequence<Pair<int, size_t>> tempSeq;

    // 3. Замер времени построения дерева
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& s : students) {
        size_t offset = tempStorage.WriteStudent(s);
        tempTree.Insert({s.birthYear, offset});
    }
    auto end = std::chrono::high_resolution_clock::now();
    result.treeBuildTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    // 4. Замер времени построения отсортированной последовательности
    FileStorage tempStorage2("benchmark_temp2.bin");
    tempStorage2.Clear();
    start = std::chrono::high_resolution_clock::now();
    for (const auto& s : students) {
        size_t offset = tempStorage2.WriteStudent(s);
        tempSeq.Add({s.birthYear, offset});
    }
    end = std::chrono::high_resolution_clock::now();
    result.sortedBuildTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    // 5. Замер времени поиска по диапазону [1995, 2005]
    Pair<int, size_t> minP(1995, 0);
    Pair<int, size_t> maxP(2005, std::numeric_limits<size_t>::max());

    start = std::chrono::high_resolution_clock::now();
    auto treeRange = tempTree.GetRange(minP, maxP);
    delete treeRange;
    end = std::chrono::high_resolution_clock::now();
    result.treeSearchTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    auto seqRange = tempSeq.GetRange(minP, maxP);
    delete seqRange;
    end = std::chrono::high_resolution_clock::now();
    result.sortedSearchTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    // Очистка временных файлов
    std::remove("benchmark_temp.bin");
    std::remove("benchmark_temp2.bin");

    return result;
}

void IndexSystem::RunFullBenchmark(const std::string& outputFile) const {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << outputFile << "\n";
        return;
    }

    // Заголовок CSV
    out << "Тип_данных; N; Время_построения_Дерево; Время_построения_Массив; Время_поиска_Дерево; Время_поиска_Массив\n";

    // Тестовые размеры
    size_t sizes[] = {1000, 10000};
    
    for (size_t N : sizes) {
        std::cout << "Тестирование N=" << N << "...\n";
        
        // Sorted Ascending
        std::cout << "  Sorted Asc... " << std::flush;
        auto res1 = RunBenchmark(N, true, true);
        out << res1.dataType << "; " << res1.dataSize << "; " 
            << res1.treeBuildTime_ms << "; " << res1.sortedBuildTime_ms << "; "
            << res1.treeSearchTime_ms << "; " << res1.sortedSearchTime_ms << "\n";
        std::cout << "готово\n";

        // Sorted Descending
        std::cout << "  Sorted Desc... " << std::flush;
        auto res2 = RunBenchmark(N, true, false);
        out << res2.dataType << "; " << res2.dataSize << "; " 
            << res2.treeBuildTime_ms << "; " << res2.sortedBuildTime_ms << "; "
            << res2.treeSearchTime_ms << "; " << res2.sortedSearchTime_ms << "\n";
        std::cout << "готово\n";

        // Random
        std::cout << "  Random... " << std::flush;
        auto res3 = RunBenchmark(N, false, false);
        out << res3.dataType << "; " << res3.dataSize << "; " 
            << res3.treeBuildTime_ms << "; " << res3.sortedBuildTime_ms << "; "
            << res3.treeSearchTime_ms << "; " << res3.sortedSearchTime_ms << "\n";
        std::cout << "готово\n";
    }
    
    out.close();
    std::cout << "\nБенчмарк завершен. Результаты сохранены в " << outputFile << "\n";
}