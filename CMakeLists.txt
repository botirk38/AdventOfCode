cmake_minimum_required(VERSION 3.10)
project(AdventOfCode2024)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add executable
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
  src/day08.cpp)
