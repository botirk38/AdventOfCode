#include "day16.h"
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

bool operator==(const State &lhs, const State &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.dir == rhs.dir;
}

std::vector<std::string>
MazeSolver::read_input(const std::string_view &file_name) {
  std::vector<std::string> maze;
  std::string line;
  std::ifstream file(file_name.data());
  while (std::getline(file, line)) {
    if (!line.empty()) {
      maze.push_back(line);
    }
  }
  return maze;
}

int MazeSolver::solve(const std::vector<std::string> &maze) {
  auto [startX, startY] = findStart(maze);
  auto [endX, endY] = findEnd(maze);

  std::priority_queue<State, std::vector<State>, std::greater<>> pq;
  std::unordered_set<State, StateHash> visited;

  pq.push({startX, startY, EAST, 0});

  const std::vector<std::pair<int, int>> DIRECTIONS = {
      {-1, 0}, // NORTH
      {0, 1},  // EAST
      {1, 0},  // SOUTH
      {0, -1}  // WEST
  };

  while (!pq.empty()) {
    State current = pq.top();
    pq.pop();

    if (current.x == endX && current.y == endY) {
      return current.cost;
    }

    if (visited.count(current))
      continue;
    visited.insert(current);

    // Try moving forward
    int newX = current.x + DIRECTIONS[current.dir].first;
    int newY = current.y + DIRECTIONS[current.dir].second;
    if (isValid(newX, newY, maze)) {
      pq.push({newX, newY, current.dir, current.cost + 1});
    }

    // Try turning left and right
    Direction leftDir = static_cast<Direction>((current.dir + 3) % 4);
    Direction rightDir = static_cast<Direction>((current.dir + 1) % 4);
    pq.push({current.x, current.y, leftDir, current.cost + 1000});
    pq.push({current.x, current.y, rightDir, current.cost + 1000});
  }

  return -1;
}

bool MazeSolver::isValid(int x, int y, const std::vector<std::string> &maze) {
  return x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() &&
         maze[x][y] != '#';
}

std::pair<int, int>
MazeSolver::findStart(const std::vector<std::string> &maze) {
  for (int i = 0; i < maze.size(); i++) {
    for (int j = 0; j < maze[i].size(); j++) {
      if (maze[i][j] == 'S')
        return {i, j};
    }
  }
  return {-1, -1};
}

std::pair<int, int> MazeSolver::findEnd(const std::vector<std::string> &maze) {
  for (int i = 0; i < maze.size(); i++) {
    for (int j = 0; j < maze[i].size(); j++) {
      if (maze[i][j] == 'E')
        return {i, j};
    }
  }
  return {-1, -1};
}

void MazeSolver::print_optimal_paths(
    const std::vector<std::string> &maze,
    const std::vector<std::vector<bool>> &is_optimal) {
  for (size_t i = 0; i < maze.size(); i++) {
    for (size_t j = 0; j < maze[i].size(); j++) {
      if (maze[i][j] == '#') {
        std::cout << '#';
      } else if (is_optimal[i][j]) {
        std::cout << 'O';
      } else {
        std::cout << '.';
      }
    }
    std::cout << '\n';
  }
}

void MazeSolver::print_maze(const std::vector<std::string> &maze) {
  for (const auto &row : maze) {
    std::cout << row << '\n';
  }
}

int MazeSolver::count_optimal_paths(const std::vector<std::string> &maze) {
  std::pair<int, int> start = findStart(maze);
  std::pair<int, int> end = findEnd(maze);

  const std::unordered_map<Direction, std::pair<int, int>> deltas = {
      {NORTH, {-1, 0}}, {EAST, {0, 1}}, {SOUTH, {1, 0}}, {WEST, {0, -1}}};

  std::unordered_map<State, int, StateHash> visited;
  std::priority_queue<std::tuple<int, Direction, int, int>> to_visit;

  visited[{start.first, start.second, EAST, 0}] = 0;
  to_visit.push({0, EAST, start.first, start.second});

  while (!to_visit.empty()) {
    auto [neg_score, dir, x, y] = to_visit.top();
    to_visit.pop();
    int score = -neg_score;

    State current{x, y, dir, 0};
    if (visited.count(current) && visited[current] < score) {
      continue;
    }

    auto [dx, dy] = deltas.at(dir);
    int nx = x + dx;
    int ny = y + dy;
    if (isValid(nx, ny, maze)) {
      State next{nx, ny, dir, 0};
      if (!visited.count(next) || visited[next] > score + 1) {
        visited[next] = score + 1;
        to_visit.push({-(score + 1), dir, nx, ny});
      }
    }

    std::array<Direction, 2> turns = {static_cast<Direction>((dir + 3) % 4),
                                      static_cast<Direction>((dir + 1) % 4)};
    for (Direction new_dir : turns) {
      State turn_state{x, y, new_dir, 0};
      if (!visited.count(turn_state) || visited[turn_state] > score + 1000) {
        visited[turn_state] = score + 1000;
        to_visit.push({-(score + 1000), new_dir, x, y});
      }
    }
  }

  int target_score = INT_MAX;
  State target_state;
  for (const auto &[state, score] : visited) {
    if (state.x == end.first && state.y == end.second && score < target_score) {
      target_score = score;
      target_state = state;
    }
  }

  std::unordered_set<std::pair<int, int>, PairHash> unique_positions;
  std::queue<State> trace_queue;
  trace_queue.push(target_state);

  while (!trace_queue.empty()) {
    State current = trace_queue.front();
    trace_queue.pop();

    unique_positions.insert({current.x, current.y});

    auto [dx, dy] = deltas.at(current.dir);
    int px = current.x - dx;
    int py = current.y - dy;

    if (isValid(px, py, maze)) {
      State prev{px, py, current.dir, 0};
      if (visited.count(prev) && visited[prev] + 1 == visited[current]) {
        trace_queue.push(prev);
      }
    }

    for (Direction prev_dir : {static_cast<Direction>((current.dir + 1) % 4),
                               static_cast<Direction>((current.dir + 3) % 4)}) {
      State prev{current.x, current.y, prev_dir, 0};
      if (visited.count(prev) && visited[prev] + 1000 == visited[current]) {
        trace_queue.push(prev);
      }
    }
  }

  return unique_positions.size();
}

void day16_part1() {
  std::vector<std::string> maze = MazeSolver::read_input("../inputs/day16.txt");
  int min_cost = MazeSolver::solve(maze);
  std::cout << "Min Cost: " << min_cost << '\n';
}

void day16_part2() {
  std::vector<std::string> maze = MazeSolver::read_input("../inputs/day16.txt");
  int optimal_paths = MazeSolver::count_optimal_paths(maze);
  std::cout << "Num Optimal Paths: " << optimal_paths << '\n';
}

