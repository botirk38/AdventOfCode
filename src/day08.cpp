#include "day08.h"
#include <ranges>
#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <set>
#include <span>
#include <string_view>
#include <thread>

namespace {
    constexpr std::string_view FILE_NAME = "../inputs/day08.txt";

    [[nodiscard]] constexpr bool is_in_bounds(const Point& p, int width, int height) noexcept {
        return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
    }
}

[[nodiscard]] std::vector<std::string> parse_input(const std::string_view& file_name) {
    std::vector<std::string> map;
    
    if (std::ifstream file(std::string(file_name)); file) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                map.emplace_back(std::move(line));
            }
        }
    }
    return map;
}

[[nodiscard]] std::vector<Point> find_antinodes(std::span<const Antenna> antennas, 
                                               int width, int height) {
    std::set<Point> antinodes;
    
    // Use parallel execution for larger datasets
    std::for_each(std::execution::par_unseq, 
                  antennas.begin(), antennas.end() - 1,
                  [&](const auto& ant1) {
        for (const auto& ant2 : std::span(antennas).subspan(&ant1 - antennas.data() + 1)) {
            if (ant1.freq != ant2.freq) continue;
            
            const auto [dx, dy] = std::tuple{ant2.x - ant1.x, ant2.y - ant1.y};
            const Point antinode1{ant1.x - dx, ant1.y - dy};
            const Point antinode2{ant2.x + dx, ant2.y + dy};
            
            #pragma omp critical
            {
                if (is_in_bounds(antinode1, width, height)) antinodes.insert(antinode1);
                if (is_in_bounds(antinode2, width, height)) antinodes.insert(antinode2);
            }
        }
    });

    return {antinodes.begin(), antinodes.end()};
}

void day08_part1() {
    const auto map = parse_input(FILE_NAME);
    if (map.empty()) {
        std::cout << "Map is empty.\n";
        return;
    }

    const auto [height, width] = std::tuple{static_cast<int>(map.size()), 
                                          static_cast<int>(map[0].size())};
    
    std::vector<Antenna> antennas;
    antennas.reserve(width * height / 4); // Estimate antenna density
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (std::isalnum(map[y][x])) {
                antennas.push_back({x, y, map[y][x]});
            }
        }
    }

    const auto antinodes = find_antinodes(antennas, width, height);
    std::cout << "Number of unique antinode locations: " << antinodes.size() << '\n';
}

[[nodiscard]] constexpr bool is_collinear(const Antenna& a1, 
                                         const Antenna& a2, 
                                         const Point& p) noexcept {
    return (p.y - a1.y) * (a2.x - a1.x) == (a2.y - a1.y) * (p.x - a1.x);
}

[[nodiscard]] std::vector<Point> find_antinodes_part2(std::span<const Antenna> antennas,
                                                     int width, int height) {
    std::set<Point> antinodes;
    const auto num_threads = std::thread::hardware_concurrency();
    std::vector<std::set<Point>> thread_antinodes(num_threads);
    
    #pragma omp parallel for collapse(2) num_threads(num_threads)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Point p{x, y};
            const auto thread_id = omp_get_thread_num();
            
            auto& local_antinodes = thread_antinodes[thread_id];
            
            for (const auto& ant1 : antennas) {
                for (const auto& ant2 : antennas) {
                    if (&ant1 >= &ant2 || ant1.freq != ant2.freq) continue;
                    
                    if (is_collinear(ant1, ant2, p)) {
                        local_antinodes.insert(p);
                        goto next_point;
                    }
                }
            }
            next_point:;
        }
    }
    
    // Merge thread-local results
    for (const auto& local_set : thread_antinodes) {
        antinodes.merge(std::set<Point>(local_set));
    }
    
    return {antinodes.begin(), antinodes.end()};
}

void day08_part2() {
    const auto map = parse_input(FILE_NAME);
    if (map.empty()) {
        std::cout << "Map is empty.\n";
        return;
    }

    const auto [height, width] = std::tuple{static_cast<int>(map.size()), 
                                          static_cast<int>(map[0].size())};
    
    std::vector<Antenna> antennas;
    antennas.reserve(width * height / 4);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (std::isalnum(map[y][x])) {
                antennas.push_back({x, y, map[y][x]});
            }
        }
    }

    const auto antinodes = find_antinodes_part2(antennas, width, height);
    std::cout << "Number of unique antinode locations (Part 2): " << antinodes.size() << '\n';
}

