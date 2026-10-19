#include "Student.hpp"
#include "IndexSystem.hpp"
#include <iostream>
#include <limits>
#include <exception>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    try {
        std::cout << "=== Student Index System ===\n";
        
        // Используем текущую папку (build), чтобы не зависеть от наличия папок data/ и results/
        IndexSystem system("students.bin"); 
        
        int choice = -1;
        while (choice != 0) {
            std::cout << "\nМеню:\n";
            std::cout << "1. Загрузить данные из CSV\n";
            std::cout << "2. Добавить студента вручную\n";
            std::cout << "3. Поиск по году рождения\n";
            std::cout << "4. Поиск по диапазону годов\n";
            std::cout << "5. Просмотр всех студентов\n";
            std::cout << "6. Запустить бенчмарк\n";
            std::cout << "0. Выход\n";
            std::cout << "Выберите действие: ";
            
            if (!(std::cin >> choice)) {
                clearInputBuffer();
                std::cout << "Некорректный ввод. Попробуйте снова.\n";
                continue;
            }

            switch (choice) {
                case 1: {
                    std::string path;
                    std::cout << "Введите путь к CSV файлу (например, ../data/students.csv): ";
                    clearInputBuffer();
                    std::getline(std::cin, path);
                    system.LoadFromCSV(path);
                    break;
                }
                case 2: {
                    std::string fName, lName, group;
                    int year;
                    std::cout << "Имя: "; clearInputBuffer(); std::getline(std::cin, fName);
                    std::cout << "Фамилия: "; std::getline(std::cin, lName);
                    std::cout << "Год рождения: "; std::cin >> year;
                    std::cout << "Группа: "; clearInputBuffer(); std::getline(std::cin, group);
                    system.AddStudent(Student(fName, lName, year, group));
                    std::cout << "Студент добавлен!\n";
                    break;
                }
                case 3: {
                    int year;
                    std::cout << "Введите год рождения: ";
                    std::cin >> year;
                    auto results = system.SearchByYear(year);
                    std::cout << "Найдено: " << results->GetLength() << " записей.\n";
                    for (size_t i = 0; i < results->GetLength(); i++) {
                        results->Get(i).print();
                    }
                    delete results;
                    break;
                }
                case 4: {
                    int minYear, maxYear;
                    std::cout << "Минимальный год: ";
                    std::cin >> minYear;
                    std::cout << "Максимальный год: ";
                    std::cin >> maxYear;
                    auto results = system.SearchByYearRange(minYear, maxYear);
                    std::cout << "Найдено: " << results->GetLength() << " записей.\n";
                    for (size_t i = 0; i < results->GetLength(); i++) {
                        results->Get(i).print();
                    }
                    delete results;
                    break;
                }
                case 5: {
                    auto results = system.GetAllStudents();
                    std::cout << "Всего записей: " << results->GetLength() << "\n";
                    for (size_t i = 0; i < results->GetLength(); i++) {
                        results->Get(i).print();
                    }
                    delete results;
                    break;
                }
                case 6: {
                    std::cout << "Запуск бенчмарка (это может занять некоторое время)...\n";
                    // Сохраняем результаты прямо в текущую папку
                    system.RunFullBenchmark("results.csv"); 
                    break;
                }
                case 0:
                    std::cout << "Выход из программы.\n";
                    break;
                default:
                    std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        }
    } 
    // Перехватываем любые ошибки, чтобы программа не падала молча
    catch (const std::exception& e) {
        std::cerr << "\n[КРИТИЧЕСКАЯ ОШИБКА]: " << e.what() << "\n";
    } 
    catch (...) {
        std::cerr << "\n[КРИТИЧЕСКАЯ ОШИБКА]: Неизвестная ошибка!\n";
    }

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.get();
    return 0;
}