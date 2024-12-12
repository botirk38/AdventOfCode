#include "day12.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>

#define FILE_NAME "../inputs/day12.txt"

const int dx[] = {0, 0, -1, 1};
const int dy[] = {1, -1, 0, 0};

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

std::vector<std::vector<char>>
read_input_day_12(const std::string_view &file_name) {
    std::vector<std::vector<char>> grid;
    std::ifstream file(file_name.data());
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            grid.push_back(std::vector<char>(line.begin(), line.end()));
        }
    }
    return grid;
}

void print_grid(const std::vector<std::vector<char>> &grid) {
    for (const auto &row : grid) {
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << '\n';
    }
}

void dfs(const std::vector<std::vector<char>> &grid,
         std::vector<std::vector<bool>> &visited, size_t row, size_t col,
         char plant, size_t &area, size_t &perimeter) {
    size_t rows = grid.size();
    size_t cols = grid[0].size();
    if (row >= rows || col >= cols || visited[row][col] ||
        grid[row][col] != plant) {
        return;
    }
    visited[row][col] = true;
    area++;
    for (int i = 0; i < 4; i++) {
        const int new_row = row + dx[i];
        const int new_col = col + dy[i];
        if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= cols ||
            grid[new_row][new_col] != plant) {
            perimeter++;
            continue;
        }
        if (!visited[new_row][new_col]) {
            dfs(grid, visited, new_row, new_col, plant, area, perimeter);
        }
    }
}

void dfs_region(const std::vector<std::vector<char>>& grid,
                std::vector<std::vector<bool>>& visited,
                std::unordered_set<std::pair<int, int>, PairHash>& region,
                int row, int col, char plant) {
    if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size() ||
        visited[row][col] || grid[row][col] != plant) {
        return;
    }
    
    visited[row][col] = true;
    region.insert({row, col});
    
    for (int i = 0; i < 4; ++i) {
        dfs_region(grid, visited, region, row + dx[i], col + dy[i], plant);
    }
}

size_t count_region_sides(const std::unordered_set<std::pair<int, int>, PairHash>& region) {
    size_t side_count = 0;
    
    for (int d = 0; d < 4; ++d) {
        std::unordered_set<std::pair<int, int>, PairHash> sides;
        
        for (const auto& pos : region) {
            std::pair<int, int> tmp = {pos.first + dx[d], pos.second + dy[d]};
            if (region.find(tmp) == region.end()) {
                sides.insert(tmp);
            }
        }
        
        std::unordered_set<std::pair<int, int>, PairHash> remove;
        for (const auto& side : sides) {
            auto tmp = std::make_pair(side.first + dy[d], side.second + dx[d]);
            while (sides.find(tmp) != sides.end()) {
                remove.insert(tmp);
                tmp = std::make_pair(tmp.first + dy[d], tmp.second + dx[d]);
            }
        }
        
        side_count += sides.size() - remove.size();
    }
    
    return side_count;
}

void day12_part1() {
    const std::vector<std::vector<char>> grid = read_input_day_12(FILE_NAME);
    if (grid.empty()) {
        std::cout << "Failed to read grid." << std::endl;
        return;
    }
    std::cout << "Grid contents:\n";
    print_grid(grid);
    size_t rows = grid.size();
    size_t cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    size_t total_price = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (!visited[i][j]) {
                size_t area = 0;
                size_t perimeter = 0;
                dfs(grid, visited, i, j, grid[i][j], area, perimeter);
                total_price += area * perimeter;
            }
        }
    }
    std::cout << "Total price: " << total_price << std::endl;
}

void day12_part2() {
    const std::vector<std::vector<char>> grid = read_input_day_12(FILE_NAME);
    if (grid.empty()) {
        std::cout << "Failed to read grid." << std::endl;
        return;
    }
    
    size_t rows = grid.size();
    size_t cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    size_t total_price = 0;
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (!visited[i][j]) {
                std::unordered_set<std::pair<int, int>, PairHash> region;
                dfs_region(grid, visited, region, i, j, grid[i][j]);
                size_t sides = count_region_sides(region);
                total_price += region.size() * sides;
            }
        }
    }
    
    std::cout << "Part 2 Total price: " << total_price << std::endl;
}

