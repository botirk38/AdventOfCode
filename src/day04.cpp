#include "day04.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>

static const std::string FILE_NAME = "../inputs/day04.txt";

// Grid position structure
struct Position {
    int x, y;
    Position(int x_, int y_) : x(x_), y(y_) {}
};

// Direction vectors for 8 possible directions
static const std::array<Position, 8> DIRECTIONS = {{
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},  {0, 1},
    {1, -1},  {1, 0},  {1, 1}
}};

// X-MAS pattern structure
struct XMASPattern {
    std::string diagonal1;
    std::string diagonal2;
    XMASPattern(const std::string& d1, const std::string& d2) 
        : diagonal1(d1), diagonal2(d2) {}
};

// Valid X-MAS patterns
static const std::array<XMASPattern, 4> VALID_PATTERNS = {{
    {"MAS", "MAS"},
    {"MAS", "SAM"},
    {"SAM", "MAS"},
    {"SAM", "SAM"}
}};

class WordGrid {
private:
    std::vector<std::string> grid;
    int rows;
    int cols;

    bool isValidPosition(const Position& pos) const {
        return pos.x >= 0 && pos.x < rows && 
               pos.y >= 0 && pos.y < cols;
    }

    std::string getWord(Position start, const Position& direction, int length) const {
        std::string word;
        for (int i = 0; i < length && isValidPosition(start); i++) {
            word += grid[start.x][start.y];
            start.x += direction.x;
            start.y += direction.y;
        }
        return word;
    }

    bool checkXPattern(const Position& center, const XMASPattern& pattern) const {
        // Check top-left to bottom-right
        Position topLeft(center.x - 1, center.y - 1);
        Position bottomRight(center.x + 1, center.y + 1);
        if (!isValidPosition(topLeft) || !isValidPosition(bottomRight)) return false;

        // Check top-right to bottom-left
        Position topRight(center.x - 1, center.y + 1);
        Position bottomLeft(center.x + 1, center.y - 1);
        if (!isValidPosition(topRight) || !isValidPosition(bottomLeft)) return false;

        std::string diag1 = std::string({
            grid[topLeft.x][topLeft.y],
            grid[center.x][center.y],
            grid[bottomRight.x][bottomRight.y]
        });

        std::string diag2 = std::string({
            grid[topRight.x][topRight.y],
            grid[center.x][center.y],
            grid[bottomLeft.x][bottomLeft.y]
        });

        return diag1 == pattern.diagonal1 && diag2 == pattern.diagonal2;
    }

public:
    WordGrid(const std::string& fileName) {
        std::ifstream file(fileName);
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                grid.push_back(line);
            }
        }
        rows = grid.size();
        cols = rows > 0 ? grid[0].size() : 0;
    }

    int countXMAS() const {
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 'X') {
                    for (const auto& dir : DIRECTIONS) {
                        if (getWord({i, j}, dir, 4) == "XMAS") {
                            count++;
                        }
                    }
                }
            }
        }
        return count;
    }

    int countXMASPart2() const {
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 'A') {
                    for (const auto& pattern : VALID_PATTERNS) {
                        if (checkXPattern({i, j}, pattern)) {
                            count++;
                        }
                    }
                }
            }
        }
        return count;
    }
};

void day04_part1() {
    WordGrid grid(FILE_NAME);
    std::cout << "Number of occurrences: " << grid.countXMAS() << std::endl;
}

void day04_part2() {
    WordGrid grid(FILE_NAME);
    std::cout << "Number of X-MAS occurrences: " << grid.countXMASPart2() << std::endl;
}

