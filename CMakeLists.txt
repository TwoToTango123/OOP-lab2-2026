cmake_minimum_required(VERSION 3.16)
project(lab02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(ENABLE_SANITIZERS "Build with AddressSanitizer and UBSan" OFF)
if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_library(mystring STATIC src/my_string.cpp)
target_include_directories(mystring PUBLIC include)
target_compile_options(mystring PRIVATE -Wall -Wextra -Wpedantic)

add_executable(lab02 src/main.cpp)
target_link_libraries(lab02 PRIVATE mystring)
target_compile_options(lab02 PRIVATE -Wall -Wextra -Wpedantic)

include(FetchContent)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(test_string tests/test_string.cpp)
target_link_libraries(test_string PRIVATE mystring GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_string)
