#include "day01.h"
#include "day02.h"
#include "day03.h"
#include "day04.h"
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
  default:
    std::cout << "Day " << day << " not implemented yet!" << std::endl;
  }

  return 0;
}
