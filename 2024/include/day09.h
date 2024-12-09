#ifndef DAY09_H
#define DAY09_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int length;
  int *blocks; // null represented as -1
} Disk;

void day09_part1(void);
void day09_part2(void);

#ifdef __cplusplus
}
#endif

#endif /* DAY09_H */
