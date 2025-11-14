cmake_minimum_required(VERSION 3.14)
project(oop_lab5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)

# Основная программа
add_executable(oop_lab5 
    main.cpp
    src/memory_resource.cpp
)
target_include_directories(oop_lab5 PRIVATE include)

# Тесты
add_executable(tests 
    tests/main_test.cpp
    tests/iterator_test.cpp
    tests/memory_resource_test.cpp
    tests/dyn_array_test.cpp
)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests gtest gtest_main)

enable_testing()
add_test(NAME oop_lab5_tests COMMAND tests)