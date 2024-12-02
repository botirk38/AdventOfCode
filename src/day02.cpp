#include "day02.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

static const std::string FILE_NAME = "../inputs/day02.txt";

std::vector<std::vector<int>> read_levels(const std::string &fileName) {
  std::ifstream file(fileName);
  std::vector<std::vector<int>> levels;

  if (!file.is_open()) {
    return levels;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::vector<int> row;
    std::istringstream iss(line);
    int num;

    while (iss >> num) {
      row.push_back(num);
    }

    if (!row.empty()) {
      levels.push_back(row);
    }
  }

  file.close();
  return levels;
}

bool is_safe(const std::vector<int> &report) {
  if (report.size() < 2) {
    return true;
  }

  bool is_increasing = report[1] > report[0];

  for (size_t i = 0; i < report.size() - 1; i++) {
    int left = report[i];
    int right = report[i + 1];

    if ((left == right) || (is_increasing ? left > right : left < right) ||
        (std::abs(left - right) > 3)) {
      return false;
    }
  }

  return true;
}

bool is_safe_part_2(const std::vector<int> &report) {

  if (is_safe(report)) {
    return true;
  }

  for (size_t i = 0; i < report.size(); i++) {

    std::vector<int> newArr;

    for (size_t j = 0; j < report.size(); j++) {

      if (j != i) {
        newArr.push_back(report[j]);
      }
    }

    if (is_safe(newArr)) {
      return true;
    }
  }

  return false;
}

void day02_part1() {
  auto levels = read_levels(FILE_NAME);
  if (levels.empty()) {
    return;
  }

  int safe_reports = 0;
  for (const auto &row : levels) {
    if (is_safe(row)) {
      safe_reports++;
    }
  }

  std::cout << "Number of safe reports: " << safe_reports << std::endl;
}

void day02_part2() {
  std::cout << "Day 2 Part 2 Solution: " << std::endl;

  auto levels = read_levels(FILE_NAME);
  if (levels.empty()) {
    return;
  }

  int safe_reports = 0;
  for (const auto &row : levels) {
    if (is_safe_part_2(row)) {
      safe_reports++;
    }
  }

  std::cout << "Number of safe reports: " << safe_reports << std::endl;
}
