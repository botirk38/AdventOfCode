#include "day10.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#define FILE_NAME "../inputs/day10.txt"

std::vector<std::string> read_input(const std::string_view &file_name) {

  std::vector<std::string> grid;

  std::ifstream file(file_name.data());

  std::string line;

  while (std::getline(file, line)) {
    if (!line.empty()) {
      grid.push_back(line);
    }
  }

  return grid;
}

void print_grid(const std::vector<std::string> &grid) {
  for (const auto &row : grid) {
    std::cout << row << std::endl;
  }
}

int count_possible_nines(const std::vector<std::string> &grid,
                         std::pair<int, int> start) {

  const int rows = grid.size();
  const int cols = grid[0].size();
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

  const int dr[] = {-1, 0, 1, 0};
  const int dc[] = {0, 1, 0, -1};

  int res = 0;

  std::queue<std::pair<int, int>> q;

  q.push(start);
  visited[start.first][start.second] = true;

  while (!q.empty()) {

    auto [row, col] = q.front();
    q.pop();

    if (grid[row][col] == '9') {
      res++;
      continue;
    }

    for (int i = 0; i < 4; i++) {
      int new_row = row + dr[i];
      int new_col = col + dc[i];

      if (new_row >= 0 && new_row < rows && new_col >= 0 && new_col < cols &&
          !visited[new_row][new_col] &&
          (grid[new_row][new_col] - grid[row][col] == 1)) {
        visited[new_row][new_col] = true;
        q.push({new_row, new_col});
      }
    }
  }

  return res;
}

void day10_part1() {

  std::vector<std::string> grid = read_input(FILE_NAME);

  if (grid.size() <= 0) {
    std::cerr << "Failed to load grid." << std::endl;
  }

  size_t rows = grid.size();
  size_t cols = grid[0].size();

  int res = 0;

  for (size_t i = 0; i < rows; i++) {

    for (size_t j = 0; j < cols; j++) {

      if (grid[i][j] == '0') {
        res += count_possible_nines(grid, {i, j});
      }
    }
  }

  std::cout << "Res: " << res << std::endl;
}

int count_unique_paths(const std::vector<std::string> &grid,
                       std::pair<int, int> curr,
                       std::vector<std::vector<bool>> &visited) {
  auto [row, col] = curr;

  if (grid[row][col] == '9') {
    return 1;
  }

  int paths_count = 0;
  const int dr[] = {-1, 0, 1, 0};
  const int dc[] = {0, 1, 0, -1};

  for (int i = 0; i < 4; i++) {
    int new_row = row + dr[i];
    int new_col = col + dc[i];

    if (new_row >= 0 && new_row < grid.size() && new_col >= 0 &&
        new_col < grid[0].size() && !visited[new_row][new_col] &&
        (grid[new_row][new_col] - grid[row][col] == 1)) {

      visited[new_row][new_col] = true;
      paths_count += count_unique_paths(grid, {new_row, new_col}, visited);
      visited[new_row][new_col] = false; // backtrack
    }
  }

  return paths_count;
}

void day10_part2() {
  auto grid = read_input(FILE_NAME);
  int total_rating = 0;

  for (int i = 0; i < grid.size(); i++) {
    for (int j = 0; j < grid[0].size(); j++) {
      if (grid[i][j] == '0') {
        std::vector<std::vector<bool>> visited(
            grid.size(), std::vector<bool>(grid[0].size(), false));
        visited[i][j] = true;
        total_rating += count_unique_paths(grid, {i, j}, visited);
      }
    }
  }

  std::cout << "Part 2 Answer: " << total_rating << std::endl;
}
