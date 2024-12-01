
#include "day01.h"
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
  default:
    std::cout << "Day " << day << " not implemented yet!" << std::endl;
  }

  return 0;
}
