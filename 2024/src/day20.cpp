#include "day20.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Day20Solver {
private:
  std::vector<std::string> grid;
  std::pair<int, int> start;
  std::pair<int, int> end;
  static constexpr std::array<std::pair<int, int>, 4> directions = {
      std::make_pair(0, 1), std::make_pair(0, -1), std::make_pair(1, 0),
      std::make_pair(-1, 0)};

  void parse(const std::string &fileName) {
    std::ifstream fin(fileName);
    std::string line;
    int r = 0;

    while (std::getline(fin, line)) {
      grid.push_back(line);
      for (int c = 0; c < line.size(); c++) {
        if (line[c] == 'S') {
          start = {r, c};
        } else if (line[c] == 'E') {
          end = {r, c};
        }
      }
      r++;
    }
  }

  std::vector<std::vector<int>> bfs(const std::vector<std::string> &grid,
                                    std::pair<int, int> start,
                                    std::pair<int, int> end) {
    std::vector<std::vector<int>> visited(
        grid.size() + 1, std::vector<int>(grid[0].size() + 1, 0));
    std::vector<std::vector<int>> dist(
        grid.size() + 1, std::vector<int>(grid[0].size() + 1, -1));

    std::queue<std::pair<std::pair<int, int>, int>> q;
    q.push({start, 0});
    visited[start.first][start.second] = 1;
    dist[start.first][start.second] = 0;

    while (!q.empty()) {
      auto [curr, d] = q.front();
      q.pop();

      if (curr == end)
        break;

      for (auto dir : directions) {
        int x = curr.first + dir.first;
        int y = curr.second + dir.second;

        if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size() ||
            visited[x][y] || grid[x][y] == '#')
          continue;

        visited[x][y] = 1;
        dist[x][y] = d + 1;
        q.push({{x, y}, d + 1});
      }
    }

    return dist;
  }

  static inline int manhattan(const std::pair<int, int> &a,
                              const std::pair<int, int> &b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
  }

  int solve(int minSteps, int saved) {
    int sol = 0;
    int n = grid.size();

    std::vector<std::vector<int>> distFromStart = bfs(grid, start, end);
    std::vector<std::vector<int>> distFromEnd = bfs(grid, end, start);

    int def = distFromStart[end.first][end.second];

    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, bool> checked;

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == '#')
          continue;

        for (int k = std::max(0, i - minSteps);
             k <= std::min(n - 1, i + minSteps); k++) {
          for (int l = std::max(0, j - minSteps);
               l <= std::min(n - 1, j + minSteps); l++) {

            if (k < 0 || k >= n || l < 0 || l >= n)
              continue;
            if (grid[k][l] == '#')
              continue;
            if (manhattan({i, j}, {k, l}) > minSteps)
              continue;

            if (distFromStart[i][j] < 0 || distFromEnd[k][l] < 0)
              continue;

            int cheat = distFromStart[i][j] + distFromEnd[k][l] +
                        manhattan({i, j}, {k, l});

            if (def - cheat >= saved) {
              sol++;
            }
          }
        }
      }
    }

    return sol;
  }

public:
  Day20Solver(const std::string &fileName) { parse(fileName); }

  int part1(int minSteps, int saved) { return solve(minSteps, saved); }

  int part2(int minSteps, int saved) { return solve(minSteps, saved); }
};

void day20_part1() {
  Day20Solver solver("../inputs/day20.txt");
  std::cout << "Part 1: " << solver.part1(2, 100) << std::endl;
}

void day20_part2() {
  Day20Solver solver("../inputs/day20.txt");
  std::cout << "Part 2: " << solver.part2(20, 100) << std::endl;
}
