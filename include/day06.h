#ifndef DAY06_H
#define DAY06_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int x;          // x coordinate
  int y;          // y coordinate
  char direction; // Current facing: '^', '>', 'v', '<'
} Guard;

typedef struct {
  char **map;
  bool **visited;
  int rows;
  int cols;
  Guard *guard;
} Lab;

void day06_part1(void);
void day06_part2(void);

#ifdef __cplusplus
}
#endif

#endif
