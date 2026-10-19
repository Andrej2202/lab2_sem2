#pragma once

#include "Student.hpp"
#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"
#include <fstream>
#include <string>
#include <cstdint>

class FileStorage {
private:
    std::string filename_;

    void WriteString(std::ofstream& file, const std::string& str);
    std::string ReadString(std::ifstream& file);

public:
    FileStorage(const std::string& filename);
    ~FileStorage();

    size_t WriteStudent(const Student& student);
    
    Student ReadStudent(size_t offset);
    
    Sequence<Student>* ReadAllStudents();
    
    // Очищает файл
    void Clear();
};