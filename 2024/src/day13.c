#include "day13.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

static long long solve_machine(long long ax, long long ay, long long bx,
                               long long by, long long px, long long py) {
  // Try all combinations up to 100 presses (as per problem constraints)
  for (int a = 0; a <= 100; a++) {
    for (int b = 0; b <= 100; b++) {
      if ((a * ax + b * bx == px) && (a * ay + b * by == py)) {
        return (3 * a + b); // Cost: 3 tokens for A, 1 token for B
      }
    }
  }
  return -1; // No solution found
}

void day13_part1(void) {
  FILE *file = fopen("../inputs/day13.txt", "r");
  if (!file) {
    printf("Error opening input file\n");
    return;
  }

  long long ax, ay, bx, by, px, py;
  long long total_tokens = 0;
  int machine_count = 0;

  while (fscanf(file,
                "Button A: X+%lld, Y+%lld\nButton B: X+%lld, Y+%lld\nPrize: "
                "X=%lld, Y=%lld\n",
                &ax, &ay, &bx, &by, &px, &py) == 6) {
    printf("Machine %d:\n", machine_count++);
    printf("Button A: X+%lld, Y+%lld\n", ax, ay);
    printf("Button B: X+%lld, Y+%lld\n", bx, by);
    printf("Prize: X=%lld, Y=%lld\n", px, py);

    long long tokens = solve_machine(ax, ay, bx, by, px, py);
    printf("Tokens needed: %lld\n\n", tokens);

    if (tokens >= 0) {
      total_tokens += tokens;
    }
  }

  fclose(file);
  printf("Part 1 - Total tokens needed: %lld\n", total_tokens);
}

static long long solve_machine_part2(long long ax, long long ay, long long bx,
                                     long long by, long long px, long long py) {
  long long x1, y1;

  // For X coordinates: ax * A + bx * B = px
  long long det = ax * by - ay * bx;
  if (det == 0)
    return -1; // No solution exists

  // Calculate A and B using Cramer's rule
  long long a = (px * by - py * bx) / det;
  long long b = (ax * py - ay * px) / det;

  // Check if we have integer solutions
  if (a * det != (px * by - py * bx) || b * det != (ax * py - ay * px))
    return -1;

  if (a < 0 || b < 0)
    return -1;

  return 3 * a + b;
}

void day13_part2(void) {
  FILE *file = fopen("../inputs/day13.txt", "r");
  if (!file) {
    printf("Error opening input file\n");
    return;
  }

  long long ax, ay, bx, by, px, py;
  long long total_tokens = 0;
  const long long OFFSET = 10000000000000LL;
  int machine_count = 0;

  while (fscanf(file,
                "Button A: X+%lld, Y+%lld\nButton B: X+%lld, Y+%lld\nPrize: "
                "X=%lld, Y=%lld\n",
                &ax, &ay, &bx, &by, &px, &py) == 6) {
    px += OFFSET;
    py += OFFSET;

    printf("Machine %d with offset:\n", machine_count++);
    long long tokens = solve_machine_part2(ax, ay, bx, by, px, py);
    printf("Tokens needed: %lld\n\n", tokens);

    if (tokens >= 0) {
      total_tokens += tokens;
    }
  }

  fclose(file);
  printf("Part 2 - Total tokens needed: %lld\n", total_tokens);
}
