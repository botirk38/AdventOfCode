#include "day15.h"
#include <complex>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Position {
  int row;
  int col;
  Position(int r, int c) : row(r), col(c) {}
};

class Warehouse {
private:
  std::vector<std::string> grid;
  Position robot;

  bool isValidMove(int row, int col) const {
    return row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size() &&
           grid[row][col] != '#';
  }

  void moveRobot(char direction) {
    int newRow = robot.row;
    int newCol = robot.col;

    // Calculate new position based on direction
    switch (direction) {
    case '^':
      newRow--;
      break;
    case 'v':
      newRow++;
      break;
    case '<':
      newCol--;
      break;
    case '>':
      newCol++;
      break;
    }

    // Check if move is valid
    if (!isValidMove(newRow, newCol))
      return;

    // Handle box pushing
    if (grid[newRow][newCol] == 'O') {
      std::vector<Position> boxesToMove;
      int checkRow = newRow;
      int checkCol = newCol;

      // Find all adjacent boxes in the push direction
      while (isValidMove(checkRow, checkCol) &&
             grid[checkRow][checkCol] == 'O') {
        boxesToMove.push_back(Position(checkRow, checkCol));
        checkRow += (newRow - robot.row);
        checkCol += (newCol - robot.col);
      }

      // Check if final position is valid
      if (!isValidMove(checkRow, checkCol))
        return;

      // Move all boxes starting from the furthest one
      for (auto it = boxesToMove.rbegin(); it != boxesToMove.rend(); ++it) {
        int targetRow = it->row + (newRow - robot.row);
        int targetCol = it->col + (newCol - robot.col);
        grid[targetRow][targetCol] = 'O';
        grid[it->row][it->col] = '.';
      }
    }

    // Move robot
    grid[newRow][newCol] = '@';
    grid[robot.row][robot.col] = '.';
    robot = Position(newRow, newCol);
  }

public:
  Warehouse(const std::vector<std::string> &input) : grid(input), robot(0, 0) {
    for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid[i].size(); j++) {
        if (grid[i][j] == '@') {
          robot = Position(i, j);
          break;
        }
      }
    }
  }

  void processMovements(const std::string &movements) {
    for (char move : movements) {
      moveRobot(move);
    }
  }

  void printGrid() const {
    for (const auto &row : grid) {
      std::cout << row << std::endl;
    }
    std::cout << "\n";
  }

  int calculateGPSSum() const {
    int sum = 0;
    for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid[i].size(); j++) {
        if (grid[i][j] == 'O') {
          sum += (100 * i + j);
        }
      }
    }
    return sum;
  }
};

void day15_part1() {
  std::ifstream file("../inputs/day15.txt");
  std::vector<std::string> grid;
  std::string line, movements;

  // Read the grid
  while (std::getline(file, line) && !line.empty()) {
    grid.push_back(line);
  }

  // Read movements
  while (std::getline(file, line)) {
    movements += line;
  }

  Warehouse warehouse(grid);
  warehouse.processMovements(movements);
  int result = warehouse.calculateGPSSum();

  std::cout << "Sum of GPS coordinates: " << result << std::endl;
}

using ComplexPos = std::complex<int>;

struct ComplexCompare {
  bool operator()(const std::complex<int> &a,
                  const std::complex<int> &b) const {
    if (a.real() != b.real())
      return a.real() < b.real();
    return a.imag() < b.imag();
  }
};

class WarehouseComplex {
private:
  std::map<std::complex<int>, char, ComplexCompare> grid;

  ComplexPos robot;

  bool move(ComplexPos p, ComplexPos d, bool test = true) {
    if (!test && !move(p, d, true))
      return false;

    std::vector<ComplexPos> todo = {p};
    if (d.imag() != 0) {
      if (grid[p] == '[')
        todo.push_back(p + ComplexPos(1, 0));
      if (grid[p] == ']')
        todo.push_back(p + ComplexPos(-1, 0));
    }

    for (ComplexPos pos : todo) {
      if (test) {
        ComplexPos next = pos + d;
        if ((grid[next] == '[' || grid[next] == ']') && !move(next, d, true) ||
            grid[next] == '#')
          return false;
      } else {
        if (grid[pos + d] == '[' || grid[pos + d] == ']')
          move(pos + d, d, false);
        std::swap(grid[pos + d], grid[pos]);
      }
    }
    return true;
  }

public:
  static std::string scaleUpMap(const std::string &input) {
    std::string scaled;
    for (char c : input) {
      switch (c) {
      case '#':
        scaled += "##";
        break;
      case '.':
        scaled += "..";
        break;
      case 'O':
        scaled += "[]";
        break;
      case '@':
        scaled += "@.";
        break;
      case '\n':
        scaled += "\n";
        break;
      default:
        scaled += c;
        break;
      }
    }
    return scaled;
  }

  WarehouseComplex(const std::string &input) {
    int row = 0, col = 0;
    for (char c : input) {
      if (c == '\n') {
        row++;
        col = 0;
      } else {
        ComplexPos pos(col, row);
        grid[pos] = c;
        if (c == '@')
          robot = pos;
        col++;
      }
    }
  }

  void processMove(char m) {
    static const std::map<char, ComplexPos> dirs = {{'<', ComplexPos(-1, 0)},
                                                    {'>', ComplexPos(1, 0)},
                                                    {'^', ComplexPos(0, -1)},
                                                    {'v', ComplexPos(0, 1)}};

    ComplexPos d = dirs.at(m);
    if (move(robot, d, false))
      robot += d;
  }

  int calculateGPSSum() const {
    int sum = 0;
    for (const auto &[pos, c] : grid) {
      if (c == '[') {
        sum += pos.real() + pos.imag() * 100;
      }
    }
    return sum;
  }
};

void day15_part2() {
  std::ifstream file("../inputs/day15.txt");
  std::string input((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

  std::string movements = input.substr(input.find("\n\n") + 2);
  std::string grid = input.substr(0, input.find("\n\n"));

  auto scaled = WarehouseComplex::scaleUpMap(grid);
  WarehouseComplex warehouse(scaled);

  for (char move : movements) {
    if (move != '\n')
      warehouse.processMove(move);
  }

  std::cout << "Sum of GPS coordinates: " << warehouse.calculateGPSSum()
            << std::endl;
}
