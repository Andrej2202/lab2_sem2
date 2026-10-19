#include "IndexSystem.hpp"

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

Sequence<Student>* IndexSystem::FetchStudents(Sequence<Pair<int, size_t>>* indexResult) const {
    MutableArraySequence<Student>* result = new MutableArraySequence<Student>();
    size_t len = indexResult->GetLength();
    for (size_t i = 0; i < len; i++) {
        Pair<int, size_t> p = indexResult->Get(i);
        result->Append(storage_->ReadStudent(p.second));
    }
    delete indexResult; // Освобождаем память индекса, так как вернули студентов
    return result;
}

void IndexSystem::LoadFromCSV(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << csvPath << "\n";
        return;
    }

    std::string line;
    // Пропускаем заголовок, если он есть (опционально, зависит от CSV)
    // std::getline(file, line); 

    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string firstName, lastName, yearStr, group;
        
        if (std::getline(ss, firstName, ',') &&
            std::getline(ss, lastName, ',') &&
            std::getline(ss, yearStr, ',') &&
            std::getline(ss, group, ',')) {
            
            Student student(firstName, lastName, std::stoi(yearStr), group);
            AddStudent(student);
            count++;
        }
    }
    std::cout << "Загружено " << count << " записей из CSV.\n";
}

void IndexSystem::AddStudent(const Student& student) {
    size_t offset = storage_->WriteStudent(student);
    Pair<int, size_t> record(student.birthYear, offset);
    treeIndex_->Insert(record);
    sortedIndex_->Add(record);
}

Sequence<Student>* IndexSystem::SearchByYear(int year) const {
    // Ищем в SortedSequence
    int index = sortedIndex_->IndexOf(Pair<int, size_t>(year, 0));
    if (index == -1) {
        return new MutableArraySequence<Student>(); // Пустой результат
    }
    
    // Поскольку могут быть дубликаты года, лучше использовать GetRange
    auto range = sortedIndex_->GetRange(Pair<int, size_t>(year, 0), Pair<int, size_t>(year, std::numeric_limits<size_t>::max()));
    return FetchStudents(range);
}

Sequence<Student>* IndexSystem::SearchByYearRange(int minYear, int maxYear) const {
    // Ищем в BinaryTree (демонстрируем работу обоих индексов)
    auto range = treeIndex_->GetRange(Pair<int, size_t>(minYear, 0), Pair<int, size_t>(maxYear, std::numeric_limits<size_t>::max()));
    return FetchStudents(range);
}

Sequence<Student>* IndexSystem::GetAllStudents() const {
    return storage_->ReadAllStudents();
}

// --- Бенчмарк ---

IndexSystem::BenchmarkResult IndexSystem::RunBenchmark(size_t N, bool isSorted, bool isAscending) const {
    BenchmarkResult result;
    result.dataSize = N;
    result.dataType = isSorted ? (isAscending ? "Sorted Asc" : "Sorted Desc") : "Random";

    // 1. Генерация данных
    std::vector<Student> students;
    students.reserve(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> yearDist(1990, 2010);
    
    for (size_t i = 0; i < N; i++) {
        students.push_back(Student("Name", "LastName", yearDist(gen), "Group"));
    }

    if (isSorted) {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.birthYear < b.birthYear;
        });
        if (!isAscending) {
            std::reverse(students.begin(), students.end());
        }
    }

    // Временное хранилище для бенчмарка
    FileStorage tempStorage("benchmark_temp.bin");
    tempStorage.Clear();
    BinaryTree<Pair<int, size_t>> tempTree;
    SortedSequence<Pair<int, size_t>> tempSeq;

    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& s : students) {
        size_t offset = tempStorage.WriteStudent(s);
        tempTree.Insert({s.birthYear, offset});
    }
    auto end = std::chrono::high_resolution_clock::now();
    result.treeBuildTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    for (const auto& s : students) {
        size_t offset = tempStorage.WriteStudent(s); // Пишем еще раз для независимого теста массива
        tempSeq.Add({s.birthYear, offset});
    }
    end = std::chrono::high_resolution_clock::now();
    result.sortedBuildTime_ms = std::chrono::duration<double, std::milli>(end - start).count();

    // Поиск диапазона (например, от 1995 до 2005)
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

    return result;
}

void IndexSystem::RunFullBenchmark(const std::string& outputFile) const {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Ошибка: не удалось создать файл " << outputFile << "\n";
        return;
    }

    out << "Тип_данных; N; Время_построения_Дерево; Время_построения_Массив; Время_поиска_Дерево; Время_поиска_Массив\n";

    size_t sizes[] = {1000, 10000}; // Можно добавить 100000, но это займет время
    
    for (size_t N : sizes) {
        auto res1 = RunBenchmark(N, true, true);
        out << res1.dataType << "; " << res1.dataSize << "; " 
            << res1.treeBuildTime_ms << "; " << res1.sortedBuildTime_ms << "; "
            << res1.treeSearchTime_ms << "; " << res1.sortedSearchTime_ms << "\n";

        auto res2 = RunBenchmark(N, true, false);
        out << res2.dataType << "; " << res2.dataSize << "; " 
            << res2.treeBuildTime_ms << "; " << res2.sortedBuildTime_ms << "; "
            << res2.treeSearchTime_ms << "; " << res2.sortedSearchTime_ms << "\n";

        auto res3 = RunBenchmark(N, false, false);
        out << res3.dataType << "; " << res3.dataSize << "; " 
            << res3.treeBuildTime_ms << "; " << res3.sortedBuildTime_ms << "; "
            << res3.treeSearchTime_ms << "; " << res3.sortedSearchTime_ms << "\n";
    }
    
    out.close();
    std::cout << "Бенчмарк завершен. Результаты сохранены в " << outputFile << "\n";
}