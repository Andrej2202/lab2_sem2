#include "FileStorage.hpp"
#include "base/Exceptions.hpp"
#include <stdexcept>

FileStorage::FileStorage(const std::string& filename) : filename_(filename) {
    std::ofstream file(filename_, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("FileStorage: Cannot open file " + filename_);
    }
}

FileStorage::~FileStorage() {}

void FileStorage::WriteString(std::ofstream& file, const std::string& str) {
    uint32_t length = static_cast<uint32_t>(str.length());
    file.write(reinterpret_cast<const char*>(&length), sizeof(uint32_t));
    if (length > 0) {
        file.write(str.c_str(), length);
    }
}

std::string FileStorage::ReadString(std::ifstream& file) {
    uint32_t length = 0;
    file.read(reinterpret_cast<char*>(&length), sizeof(uint32_t));
    
    if (file.eof() || file.fail()) {
        throw EndOfStreamException("FileStorage", "ReadString", "Unexpected end of file");
    }

    std::string str(length, '\0');
    if (length > 0) {
        file.read(&str[0], length);
    }
    return str;
}

size_t FileStorage::WriteStudent(const Student& student) {
    std::ofstream file(filename_, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("FileStorage: Cannot open file for writing");
    }

    size_t offset = static_cast<size_t>(file.tellp());

    WriteString(file, student.firstName);
    WriteString(file, student.lastName);
    
    int32_t birthYear = student.birthYear;
    file.write(reinterpret_cast<const char*>(&birthYear), sizeof(int32_t));
    
    WriteString(file, student.group);

    return offset;
}

Student FileStorage::ReadStudent(size_t offset) {
    std::ifstream file(filename_, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("FileStorage: Cannot open file for reading");
    }


    file.seekg(static_cast<std::streamoff>(offset));
    if (file.fail()) {
        throw IndexOutOfRangeException("FileStorage", "ReadStudent", "Invalid offset");
    }

    Student student;
    student.firstName = ReadString(file);
    student.lastName = ReadString(file);
    
    int32_t birthYear = 0;
    file.read(reinterpret_cast<char*>(&birthYear), sizeof(int32_t));
    student.birthYear = birthYear;
    
    student.group = ReadString(file);

    return student;
}

Sequence<Student>* FileStorage::ReadAllStudents() {
    MutableArraySequence<Student>* students = new MutableArraySequence<Student>();
    
    std::ifstream file(filename_, std::ios::binary);
    if (!file.is_open()) {
        return students;
    }

    while (file.peek() != EOF) {
        size_t currentOffset = static_cast<size_t>(file.tellg());
        try {
            Student student = ReadStudent(currentOffset);
            students->Append(student);
        } catch (const EndOfStreamException&) {
            break;
        }
    }

    return students;
}

void FileStorage::Clear() {
    std::ofstream file(filename_, std::ios::binary | std::ios::trunc);
}