#include "day18.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

const int GRID_SIZE = 71; // 0 to 70 inclusive

struct Point {
  int x, y;
  Point(int _x, int _y) : x(_x), y(_y) {}
};

struct Node {
  int x, y, dist;
  Node(int _x, int _y, int _d) : x(_x), y(_y), dist(_d) {}

  bool operator>(const Node &other) const { return dist > other.dist; }
};

std::vector<Point> readInput() {
  std::vector<Point> points;
  std::ifstream file("../inputs/day18.txt");
  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    int x, y;
    char comma;
    ss >> x >> comma >> y;
    points.emplace_back(x, y);
  }

  return points;
}

int findShortestPath(const std::vector<std::vector<bool>> &corrupted) {
  std::vector<std::vector<int>> dist(GRID_SIZE,
                                     std::vector<int>(GRID_SIZE, INT_MAX));
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

  // Directions: right, down, left, up
  const int dx[] = {1, 0, -1, 0};
  const int dy[] = {0, 1, 0, -1};

  dist[0][0] = 0;
  pq.emplace(0, 0, 0);

  while (!pq.empty()) {
    Node curr = pq.top();
    pq.pop();

    if (curr.x == GRID_SIZE - 1 && curr.y == GRID_SIZE - 1) {
      return curr.dist;
    }

    if (curr.dist > dist[curr.y][curr.x])
      continue;

    for (int i = 0; i < 4; i++) {
      int nx = curr.x + dx[i];
      int ny = curr.y + dy[i];

      if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE &&
          !corrupted[ny][nx] && dist[ny][nx] > curr.dist + 1) {
        dist[ny][nx] = curr.dist + 1;
        pq.emplace(nx, ny, curr.dist + 1);
      }
    }
  }

  return -1; // No path found
}

void printGrid(const std::vector<std::vector<bool>> &corrupted) {
  for (int y = 0; y < GRID_SIZE; y++) {
    for (int x = 0; x < GRID_SIZE; x++) {
      std::cout << (corrupted[y][x] ? '#' : '.');
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

void day18_part1() {
  std::vector<Point> points = readInput();
  std::vector<std::vector<bool>> corrupted(GRID_SIZE,
                                           std::vector<bool>(GRID_SIZE, false));

  // Mark first 1024 bytes as corrupted
  for (size_t i = 0; i < std::min(size_t(1024), points.size()); i++) {
    corrupted[points[i].y][points[i].x] = true;
  }

  std::cout << "Shortest Path: " << findShortestPath(corrupted) << '\n';
}

void day18_part2() {
  std::vector<std::vector<bool>> corrupted(GRID_SIZE,
                                           std::vector<bool>(GRID_SIZE, false));
  std::vector<Point> points = readInput();

  // Pre-check optimization: Only test points that could potentially block paths
  std::vector<bool> visited(GRID_SIZE * GRID_SIZE, false);
  auto isPathPoint = [&](int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE &&
           !corrupted[y][x] && !visited[y * GRID_SIZE + x];
  };

  // Find initial valid path
  std::queue<Point> q;
  q.emplace(0, 0);
  std::vector<Point> criticalPath;

  while (!q.empty()) {
    Point curr = q.front();
    q.pop();

    if (curr.x == GRID_SIZE - 1 && curr.y == GRID_SIZE - 1) {
      break;
    }

    if (!visited[curr.y * GRID_SIZE + curr.x]) {
      visited[curr.y * GRID_SIZE + curr.x] = true;
      criticalPath.push_back(curr);

      // Check adjacent cells
      const int dx[] = {1, 0, -1, 0};
      const int dy[] = {0, 1, 0, -1};
      for (int i = 0; i < 4; i++) {
        int nx = curr.x + dx[i];
        int ny = curr.y + dy[i];
        if (isPathPoint(nx, ny)) {
          q.emplace(nx, ny);
        }
      }
    }
  }

  // Only test points near the critical path
  for (const Point &p : points) {
    // Check if point is adjacent to critical path
    bool isNearPath = false;
    for (const Point &cp : criticalPath) {
      if (abs(p.x - cp.x) <= 1 && abs(p.y - cp.y) <= 1) {
        isNearPath = true;
        break;
      }
    }

    if (!isNearPath)
      continue;

    corrupted[p.y][p.x] = true;
    int result = findShortestPath(corrupted);

    if (result == -1) {
      std::cout << "First Byte to block exit: " << p.x << "," << p.y << '\n';
      return;
    }
  }
}
