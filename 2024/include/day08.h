

#pragma once

struct Antenna {
  int x, y;
  char freq;
};

struct Point {
  int x, y;
  bool operator<(const Point &other) const {
    return x < other.x || (x == other.x && y < other.y);
  }
};

void day08_part1();
void day08_part2();
