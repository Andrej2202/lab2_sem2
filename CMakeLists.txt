cmake_minimum_required(VERSION 3.10)
project(StudentIndexSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Статическая линковка для MinGW
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/base
)

# Основная цель
add_executable(student_system
    src/main.cpp
    src/Student.cpp
    src/FileStorage.cpp
    # src/IndexSystem.cpp
)

# Тестовая цель
add_executable(test_system
    tests/test_main.cpp
    src/Student.cpp
    src/FileStorage.cpp
    # src/IndexSystem.cpp
)


if(MSVC)
    target_compile_options(student_system PRIVATE /W4)
    target_compile_options(test_system PRIVATE /W4)
else()
    target_compile_options(student_system PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(test_system PRIVATE -Wall -Wextra -pedantic)
endif()