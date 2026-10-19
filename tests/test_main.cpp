#include "test_student.hpp"
#include "test_pair.hpp"
#include "test_sorted_sequence.hpp"
#include "test_binary_tree.hpp"
#include "test_file_storage.hpp"
#include "test_index_system.hpp"

#include <iostream>

int main() {
    std::cout << "========================================\n";
    std::cout << "  ЗАПУСК ВСЕХ ТЕСТОВ\n";
    std::cout << "========================================\n\n";
    
    try {
        test_student();
        test_pair();
        test_sorted_sequence();
        test_binary_tree();
        test_file_storage();
        test_index_system();
        
        std::cout << "========================================\n";
        std::cout << "  ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! \n";
        std::cout << "========================================\n";
        
        return 0;
    } 
    catch (const std::exception& e) {
        std::cerr << "\n[ОШИБКА]: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "\n[ОШИБКА]: Неизвестное исключение!\n";
        return 1;
    }
}