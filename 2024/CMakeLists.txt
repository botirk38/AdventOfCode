cmake_minimum_required(VERSION 3.10)
project(AdventOfCode2024)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenMP_C "${CMAKE_C_COMPILER}")

# macOS specific OpenMP configuration
if(APPLE)
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)

  if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(OpenMP_C "${CMAKE_C_COMPILER}")
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY /opt/homebrew/opt/libomp/lib/libomp.dylib)
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY /opt/homebrew/opt/libomp/lib/libomp.dylib)
  endif()
endif()

find_package(OpenMP REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/opt/homebrew/opt/libomp/include)

add_executable(
  aoc2024
  src/main.cpp
  src/day01.c
  src/day02.cpp
  src/day03.c
  src/day04.cpp
  src/day05.cpp
  src/day06.c
  src/day07.c
  src/day08.cpp
  src/day09.c
  src/day10.cpp
  src/day11.cpp
  src/day12.cpp
  src/day13.c
  src/day14.cpp
  src/day15.cpp
  src/day16.cpp
  src/day17.c)

target_link_libraries(aoc2024 PRIVATE OpenMP::OpenMP_CXX)
