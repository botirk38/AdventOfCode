#include "day05.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <vector>

namespace {
constexpr std::string_view FILE_NAME = "../inputs/day05.txt";

std::vector<int> parseSequence(std::string_view line) {
  std::vector<int> res;
  res.reserve(std::count(line.begin(), line.end(), ',') + 1);

  size_t pos = 0;
  size_t next;
  while ((next = line.find(',', pos)) != std::string_view::npos) {
    res.push_back(std::stoi(std::string(line.substr(pos, next - pos))));
    pos = next + 1;
  }
  res.push_back(std::stoi(std::string(line.substr(pos))));
  return res;
}

bool isValidSequence(std::span<const int> sequence,
                     const std::set<std::pair<int, int>> &rules) {
  for (size_t i = 0; i < sequence.size(); ++i) {
    for (size_t j = i + 1; j < sequence.size(); ++j) {
      if (rules.contains({sequence[j], sequence[i]})) {
        return false;
      }
    }
  }
  return true;
}

std::vector<int> getCorrectOrder(std::span<const int> sequence,
                                 const std::set<std::pair<int, int>> &rules) {
  std::vector<int> result{sequence.begin(), sequence.end()};

  std::stable_sort(
      result.begin(), result.end(), [&rules](const int a, const int b) {
        return rules.contains({a, b}) || (!rules.contains({b, a}) && a < b);
      });

  return result;
}

struct InputData {
  std::set<std::pair<int, int>> rules;
  std::vector<std::vector<int>> updates;

  static InputData parse(const std::string &filename) {
    InputData data;
    std::ifstream file(filename);
    std::string line;
    bool parsingRules = true;

    while (std::getline(file, line)) {
      if (line.empty()) {
        parsingRules = false;
        continue;
      }

      if (parsingRules) {
        auto delimPos = line.find('|');
        data.rules.emplace(std::stoi(line.substr(0, delimPos)),
                           std::stoi(line.substr(delimPos + 1)));
      } else {
        data.updates.push_back(parseSequence(line));
      }
    }
    return data;
  }
};
} // namespace

void day05_part1() {
  const auto data = InputData::parse(std::string(FILE_NAME));
  int sum = 0;

  for (const auto &update : data.updates) {
    if (isValidSequence(update, data.rules)) {
      sum += update[update.size() / 2];
    }
  }

  std::cout << "Sum of Middle Elements Part 1: " << sum << '\n';
}

void day05_part2() {
  const auto data = InputData::parse(std::string(FILE_NAME));
  int sum = 0;

  for (const auto &update : data.updates) {
    if (!isValidSequence(update, data.rules)) {
      auto correctOrder = getCorrectOrder(update, data.rules);
      sum += correctOrder[correctOrder.size() / 2];
    }
  }

  std::cout << "Sum of middle elements in corrected sequences Part 2: " << sum
            << '\n';
}

