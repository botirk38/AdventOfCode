#include "day14.h"
#include <iostream>
#include <array>
#include <cstdio>
#include <string>
#include <chrono>

constexpr int WIDTH = 101;
constexpr int HEIGHT = 103;
constexpr int SECONDS = 100;
constexpr const char* FILE_NAME = "../inputs/day14.txt";

static int positive_mod(int x, int m) {
    return x % m < 0 ? x % m + m : x % m;
}

void day14_part1() {
      auto start = std::chrono::high_resolution_clock::now();

    int px, py, vx, vy;
    std::array<std::array<int, 2>, 2> count = {};
    
    FILE* fp = std::fopen(FILE_NAME, "r");
    
    int particle_count = 0;
    while(std::fscanf(fp, "p=%d,%d v=%d,%d\n", &px, &py, &vx, &vy) == 4) {
        
        px = positive_mod((px + vx * SECONDS), WIDTH);
        py = positive_mod((py + vy * SECONDS), HEIGHT);
        
        
        if (px != WIDTH/2 && py != HEIGHT/2) {
            int quadrant_x = px / (WIDTH/2 + 1);
            int quadrant_y = py / (HEIGHT/2 + 1);
            count[quadrant_x][quadrant_y]++;
        } 
  }
    std::fclose(fp);
    
    
    const int safety_factor = count[0][0] * count[0][1] * count[1][0] * count[1][1];
    
    std::cout << "Day 14 Part 1: Safety factor = " << safety_factor << '\n';
  auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " microseconds\n";

}




void day14_part2() {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::pair<int, int>> positions;
    std::vector<std::pair<int, int>> velocities;
    
    FILE* fp = std::fopen(FILE_NAME, "r");
    int px, py, vx, vy;
    
    while(std::fscanf(fp, "p=%d,%d v=%d,%d\n", &px, &py, &vx, &vy) == 4) {
        positions.emplace_back(px, py);
        velocities.emplace_back(vx, vy);
    }
    std::fclose(fp);

    // Find best x and y offsets by calculating variances
    double min_var_x = std::numeric_limits<double>::max();
    double min_var_y = std::numeric_limits<double>::max();
    int best_x = 0, best_y = 0;

    for (int t = 0; t < WIDTH; ++t) {
        double mean_x = 0;
        std::vector<int> curr_x;
        
        for (size_t i = 0; i < positions.size(); ++i) {
            int x = positions[i].first + velocities[i].first * t;
            x = positive_mod(x, WIDTH);
            curr_x.push_back(x);
            mean_x += x;
        }
        mean_x /= positions.size();

        double variance_x = 0;
        for (int x : curr_x) {
            variance_x += (x - mean_x) * (x - mean_x);
        }

        if (variance_x < min_var_x) {
            min_var_x = variance_x;
            best_x = t;
        }
    }

    for (int t = 0; t < HEIGHT; ++t) {
        double mean_y = 0;
        std::vector<int> curr_y;
        
        for (size_t i = 0; i < positions.size(); ++i) {
            int y = positions[i].second + velocities[i].second * t;
            y = positive_mod(y, HEIGHT);
            curr_y.push_back(y);
            mean_y += y;
        }
        mean_y /= positions.size();

        double variance_y = 0;
        for (int y : curr_y) {
            variance_y += (y - mean_y) * (y - mean_y);
        }

        if (variance_y < min_var_y) {
            min_var_y = variance_y;
            best_y = t;
        }
    }

    // Calculate modular multiplicative inverse of WIDTH mod HEIGHT
    int w_inv = 1;
    for (int i = 1; i < HEIGHT; ++i) {
        if ((WIDTH * i) % HEIGHT == 1) {
            w_inv = i;
            break;
        }
    }

    // Calculate final time using CRT
    int k = ((w_inv * (best_y - best_x)) % HEIGHT + HEIGHT) % HEIGHT;
    long long result = best_x + k * WIDTH;

    std::cout << "Day 14 Part 2: Time to display Easter egg = " << result << '\n';
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " microseconds\n";
}

