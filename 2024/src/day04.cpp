#include "day04.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static const std::string FILE_NAME = "../inputs/day04.txt";

std::vector<std::string> read_words(const std::string &fileName) {
  std::ifstream file(fileName);
  std::vector<std::string> words;

  if (!file.is_open()) {
    return words;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty()) {
      words.push_back(line);
    }
  }

  file.close();
  return words;
}

int countXmas(const std::vector<std::string> &grid) {
  if (grid.empty()) {
    return 0;
  }

  int rows = grid.size();
  int cols = grid[0].size();
  const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int count = 0;

  auto isValidEntry = [&](int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
  };

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[i][j] == 'X') {
        for (int dir = 0; dir < 8; dir++) {
          int x = i;
          int y = j;
          std::string word;

          for (int len = 0; len < 4; len++) {
            if (!isValidEntry(x, y))
              break;
            word += grid[x][y];
            x += dx[dir];
            y += dy[dir];
          }

          if (word == "XMAS") {
            count++;
          }
        }
      }
    }
  }
  return count;
}

int countXmasPart2(const std::vector<std::string> &grid) {
  if (grid.empty()) {
    return 0;
  }

  int rows = grid.size();
  int cols = grid[0].size();
  int count = 0;

  auto isValidEntry = [&](int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
  };

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[i][j] == 'A') {

        const std::vector<std::pair<std::string, std::string>> validCombos = {
            {"MAS", "MAS"}, {"MAS", "SAM"}, {"SAM", "MAS"}, {"SAM", "SAM"}};

        for (const auto &[pattern1, pattern2] : validCombos) {
          bool match = true;

          // Check first diagonal (top-left to bottom-right)
          if (!(isValidEntry(i - 1, j - 1) && isValidEntry(i + 1, j + 1)))
            continue;
          if (grid[i - 1][j - 1] != pattern1[0] || grid[i][j] != pattern1[1] ||
              grid[i + 1][j + 1] != pattern1[2])
            continue;

          // Check second diagonal (top-right to bottom-left)
          if (!(isValidEntry(i - 1, j + 1) && isValidEntry(i + 1, j - 1)))
            continue;
          if (grid[i - 1][j + 1] != pattern2[0] || grid[i][j] != pattern2[1] ||
              grid[i + 1][j - 1] != pattern2[2])
            continue;

          count++;
        }
      }
    }
  }

  return count;
}

void day04_part1() {
  std::vector<std::string> grid = read_words(FILE_NAME);
  int number_of_xmas_occurrences = countXmas(grid);
  std::cout << "Number of occurrences: " << number_of_xmas_occurrences
            << std::endl;
}

void day04_part2() {
  std::vector<std::string> grid = read_words(FILE_NAME);
  int number_of_xmas_occurrences = countXmasPart2(grid);
  std::cout << "Number of occurrences: " << number_of_xmas_occurrences
            << std::endl;
}

