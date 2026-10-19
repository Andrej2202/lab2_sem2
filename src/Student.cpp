#include "../include/Student.hpp"

Student::Student() 
    : firstName(""), lastName(""), birthYear(0), group("") {}

Student::Student(const std::string& firstName, const std::string& lastName, int birthYear, const std::string& group)
    : firstName(firstName), lastName(lastName), birthYear(birthYear), group(group) {}

void Student::print() const {
    std::cout << lastName << " " << firstName 
              << " | Год рождения: " << birthYear 
              << " | Группа: " << group << "\n";
}