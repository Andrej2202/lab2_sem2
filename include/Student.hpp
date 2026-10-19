#pragma once

#include <string>
#include <iostream>

struct Student {
    std::string firstName;
    std::string lastName;
    int birthYear;
    std::string group;

    Student();
    Student(const std::string& firstName, const std::string& lastName, int birthYear, const std::string& group);

    void print() const;
};