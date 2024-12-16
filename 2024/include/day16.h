#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

enum Direction { NORTH, EAST, SOUTH, WEST };

struct State {
  int x, y;
  Direction dir;
  int cost;
  bool operator>(const State &other) const { return cost > other.cost; }
};

bool operator==(const State &lhs, const State &rhs);

struct PairHash {
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

struct StateHash {
  size_t operator()(const State &s) const {
    return std::hash<int>()(s.x) ^ (std::hash<int>()(s.y) << 1) ^
           (std::hash<int>()(static_cast<int>(s.dir)) << 2);
  }
};

class MazeSolver {
public:
  static std::vector<std::string> read_input(const std::string_view &file_name);
  static int solve(const std::vector<std::string> &grid);
  static int count_optimal_paths(const std::vector<std::string> &maze);
  static void print_maze(const std::vector<std::string> &maze);
  static void
  print_optimal_paths(const std::vector<std::string> &maze,
                      const std::vector<std::vector<bool>> &is_optimal);

private:
  static bool isValid(int x, int y, const std::vector<std::string> &maze);
  static std::pair<int, int> findStart(const std::vector<std::string> &maze);
  static std::pair<int, int> findEnd(const std::vector<std::string> &maze);
};

void day16_part1();
void day16_part2();

