#include "day01.h"
#include "day02.h"
#include "day03.h"
#include "day04.h"
#include "day05.h"
#include "day06.h"
#include "day07.h"
#include "day08.h"
#include "day09.h"
#include "day10.h"
#include "day11.h"
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <day_number>" << std::endl;
    return 1;
  }

  int day = std::atoi(argv[1]);

  switch (day) {
  case 1:
    day01_part1();
    day01_part2();
    break;

  case 2:
    day02_part1();
    day02_part2();
    break;

  case 3:
    day03_part1();
    day03_part2();
    break;

  case 4:
    day04_part1();
    day04_part2();
    break;
  case 5:
    day05_part1();
    day05_part2();
    break;

  case 6:
    day06_part1();
    day06_part2();
    break;

  case 7:
    day07_part1();
    day07_part2();
    break;

  case 8:
    day08_part1();
    day08_part2();
    break;

  case 9:
    day09_part1();
    day09_part2();
    break;
  case 10:
    day10_part1();
    day10_part2();
    break;
  case 11:
    day11_part1();
    day11_part2();
  default:
    std::cout << "Day " << day << " not implemented yet!" << std::endl;
  }

  return 0;
}
