#pragma once

#include "Student.hpp"
#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"
#include "base/Exceptions.hpp"
#include <fstream>
#include <string>
#include <cstdint>
#include <stdexcept>

class FileStorage {
private:
    std::string filename_;
    
    void WriteString(std::ostream& file, const std::string& str) const;
    std::string ReadString(std::istream& file) const;

public:
    FileStorage(const std::string& filename);
    ~FileStorage();

    size_t WriteStudent(const Student& student);
    Student ReadStudent(size_t offset) const;
    Sequence<Student>* ReadAllStudents() const;
    void Clear();
};