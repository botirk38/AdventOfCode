#include "day11.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

static constexpr const char *FILE_NAME = "../inputs/day11.txt";

using Cache = std::unordered_map<std::pair<long long, int>, long long>;

namespace std {
template <> struct hash<std::pair<long long, int>> {
  size_t operator()(const std::pair<long long, int> &p) const {
    return std::hash<long long>()(p.first) ^ std::hash<int>()(p.second);
  }
};
} // namespace std

static std::vector<long long> read_input_file(const std::string &file_name) {
  std::ifstream file(file_name);
  if (!file) {
    throw std::runtime_error("Failed to open file.");
  }
  std::vector<long long> numbers;
  long long num;
  while (file >> num) {
    numbers.push_back(num);
  }
  return numbers;
}

static long long process_blinks(long long stone, int blinks, Cache &memory) {
  if (blinks == 0) {
    return 1;
  }

  std::pair<long long, int> key = std::make_pair(stone, blinks);

  if (auto it = memory.find(key); it != memory.end()) {
    return it->second;
  }

  long long result;
  if (stone == 0) {
    result = process_blinks(1, blinks - 1, memory);
  } else {
    std::string str_stone = std::to_string(stone);
    if (str_stone.length() % 2 == 0) {
      size_t mid = str_stone.length() / 2;
      long long left = std::stoll(str_stone.substr(0, mid));
      long long right = std::stoll(str_stone.substr(mid));
      result = process_blinks(left, blinks - 1, memory) +
               process_blinks(right, blinks - 1, memory);
    } else {
      result = process_blinks(stone * 2024, blinks - 1, memory);
    }
  }

  memory[key] = result;
  return result;
}

void day11_part1(void) {
  std::vector<long long> numbers = read_input_file(FILE_NAME);
  Cache memory;
  long long total = 0;
  for (auto stone : numbers) {
    total += process_blinks(stone, 25, memory);
  }
  std::cout << "Number of stones after 25 blinks: " << total << std::endl;
}

void day11_part2(void) {
  std::vector<long long> numbers = read_input_file(FILE_NAME);
  Cache memory;
  long long total = 0;
  for (auto stone : numbers) {
    total += process_blinks(stone, 75, memory);
  }
  std::cout << "Number of stones after 75 blinks: " << total << std::endl;
}
