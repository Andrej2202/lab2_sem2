#include "IndexSystem.hpp"
#include <QTableWidget>
#include <QString>
#include <cstring>

void IndexSystem::SearchByYearAndPopulate(int year, QTableWidget* table) const {
    if (!table) return;
    
    try {
        auto range = sortedIndex_->GetRange(
            Pair<int, size_t>(year, 0),
            Pair<int, size_t>(year, std::numeric_limits<size_t>::max())
        );
        
        size_t len = range->GetLength();
        table->setRowCount(static_cast<int>(len));
        
        for (size_t i = 0; i < len; i++) {
            try {
                Pair<int, size_t> p = range->Get(i);
                Student s = storage_->ReadStudent(p.second);
                
                char firstNameBuf[256] = {0};
                char lastNameBuf[256] = {0};
                char groupBuf[256] = {0};
                
                size_t firstNameLen = std::min(s.firstName.size(), sizeof(firstNameBuf) - 1);
                size_t lastNameLen = std::min(s.lastName.size(), sizeof(lastNameBuf) - 1);
                size_t groupLen = std::min(s.group.size(), sizeof(groupBuf) - 1);
                
                memcpy(firstNameBuf, s.firstName.data(), firstNameLen);
                memcpy(lastNameBuf, s.lastName.data(), lastNameLen);
                memcpy(groupBuf, s.group.data(), groupLen);
                
                QString firstName = QString::fromUtf8(firstNameBuf, firstNameLen);
                QString lastName = QString::fromUtf8(lastNameBuf, lastNameLen);
                QString group = QString::fromUtf8(groupBuf, groupLen);
                
                table->setItem(i, 0, new QTableWidgetItem(firstName));
                table->setItem(i, 1, new QTableWidgetItem(lastName));
                table->setItem(i, 2, new QTableWidgetItem(QString::number(s.birthYear)));
                table->setItem(i, 3, new QTableWidgetItem(group));
            } catch (...) {}
        }
        
        delete range;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка поиска: " << e.what() << "\n";
    }
}

void IndexSystem::SearchByYearRangeAndPopulate(int minYear, int maxYear, QTableWidget* table) const {
    if (!table) return;
    
    try {
        auto range = treeIndex_->GetRange(
            Pair<int, size_t>(minYear, 0),
            Pair<int, size_t>(maxYear, std::numeric_limits<size_t>::max())
        );
        
        size_t len = range->GetLength();
        table->setRowCount(static_cast<int>(len));
        
        for (size_t i = 0; i < len; i++) {
            try {
                Pair<int, size_t> p = range->Get(i);
                Student s = storage_->ReadStudent(p.second);
                
                char firstNameBuf[256] = {0};
                char lastNameBuf[256] = {0};
                char groupBuf[256] = {0};
                
                size_t firstNameLen = std::min(s.firstName.size(), sizeof(firstNameBuf) - 1);
                size_t lastNameLen = std::min(s.lastName.size(), sizeof(lastNameBuf) - 1);
                size_t groupLen = std::min(s.group.size(), sizeof(groupBuf) - 1);
                
                memcpy(firstNameBuf, s.firstName.data(), firstNameLen);
                memcpy(lastNameBuf, s.lastName.data(), lastNameLen);
                memcpy(groupBuf, s.group.data(), groupLen);
                
                QString firstName = QString::fromUtf8(firstNameBuf, firstNameLen);
                QString lastName = QString::fromUtf8(lastNameBuf, lastNameLen);
                QString group = QString::fromUtf8(groupBuf, groupLen);
                
                table->setItem(i, 0, new QTableWidgetItem(firstName));
                table->setItem(i, 1, new QTableWidgetItem(lastName));
                table->setItem(i, 2, new QTableWidgetItem(QString::number(s.birthYear)));
                table->setItem(i, 3, new QTableWidgetItem(group));
            } catch (...) {}
        }
        
        delete range;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка поиска по диапазону: " << e.what() << "\n";
    }
}

void IndexSystem::ShowAllAndPopulate(QTableWidget* table) const {
    if (!table) return;
    
    try {
        auto students = storage_->ReadAllStudents();
        size_t len = students->GetLength();
        table->setRowCount(static_cast<int>(len));
        
        for (size_t i = 0; i < len; i++) {
            try {
                Student s = students->Get(i);
                
                char firstNameBuf[256] = {0};
                char lastNameBuf[256] = {0};
                char groupBuf[256] = {0};
                
                size_t firstNameLen = std::min(s.firstName.size(), sizeof(firstNameBuf) - 1);
                size_t lastNameLen = std::min(s.lastName.size(), sizeof(lastNameBuf) - 1);
                size_t groupLen = std::min(s.group.size(), sizeof(groupBuf) - 1);
                
                memcpy(firstNameBuf, s.firstName.data(), firstNameLen);
                memcpy(lastNameBuf, s.lastName.data(), lastNameLen);
                memcpy(groupBuf, s.group.data(), groupLen);
                
                QString firstName = QString::fromUtf8(firstNameBuf, firstNameLen);
                QString lastName = QString::fromUtf8(lastNameBuf, lastNameLen);
                QString group = QString::fromUtf8(groupBuf, groupLen);
                
                table->setItem(i, 0, new QTableWidgetItem(firstName));
                table->setItem(i, 1, new QTableWidgetItem(lastName));
                table->setItem(i, 2, new QTableWidgetItem(QString::number(s.birthYear)));
                table->setItem(i, 3, new QTableWidgetItem(group));
            } catch (...) {}
        }
        
        delete students;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка чтения всех студентов: " << e.what() << "\n";
    }
}