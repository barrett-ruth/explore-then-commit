cmake_minimum_required(VERSION 3.20)
project(ExploreThenCommit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC_FILES src/*.cc)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

option(ENABLE_TESTS "Enable unit tests" ON)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_executable(artie src/artie.cc)
target_include_directories(artie PRIVATE include)
