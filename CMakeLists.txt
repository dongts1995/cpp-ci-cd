cmake_minimum_required(VERSION 3.20)

project(CppCiCdProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Main application
# =========================

add_executable(cpp_ci_cd
    src/main.cpp
    src/Calculator.cpp
)

target_include_directories(cpp_ci_cd
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# =========================
# GoogleTest
# =========================

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

# =========================
# Tests
# =========================

enable_testing()

add_executable(unit_tests
    tests/CalculatorTest.cpp
    src/Calculator.cpp
)

target_include_directories(unit_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(unit_tests
    PRIVATE
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(unit_tests)