#include "day01.h"
#include <stdio.h>
#include <stdlib.h>

static const char *FILE_NAME = "../inputs/day01.txt";

// Compare function for qsort
int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

// Common function to read input data
void read_input(int **left, int **right, int *size, int *max_num) {
  FILE *file = fopen(FILE_NAME, "r");
  if (file == NULL) {
    printf("Error: Could not open file\n");
    return;
  }

  int cap = 10;
  *size = 0;
  *max_num = 0;
  int left_n, right_n;

  *left = (int *)malloc(cap * sizeof(int));
  *right = (int *)malloc(cap * sizeof(int));

  while (fscanf(file, "%d %d", &left_n, &right_n) == 2) {
    if (*size >= cap) {
      cap *= 2;
      *left = (int *)realloc(*left, cap * sizeof(int));
      *right = (int *)realloc(*right, cap * sizeof(int));
    }

    (*left)[*size] = left_n;
    (*right)[*size] = right_n;
    *max_num = (left_n > *max_num) ? left_n : *max_num;
    *max_num = (right_n > *max_num) ? right_n : *max_num;
    (*size)++;
  }

  fclose(file);
}

void day01_part1(void) {
  printf("Day 1, Part 1 solution\n");
  int *left, *right, size, max_num;

  read_input(&left, &right, &size, &max_num);

  qsort(left, size, sizeof(int), compare);
  qsort(right, size, sizeof(int), compare);

  long long dist = 0;
  for (int i = 0; i < size; i++) {
    dist += abs(left[i] - right[i]);
  }

  printf("Total Distance: %lld\n", dist);

  free(left);
  free(right);
}

void day01_part2(void) {
  printf("Day 1, Part 2 solution\n");
  int *left, *right, size, max_num;

  read_input(&left, &right, &size, &max_num);

  int *freq = (int *)calloc(max_num + 1, sizeof(int));
  for (int i = 0; i < size; i++) {
    freq[right[i]]++;
  }

  long long similarity_score = 0;
  for (int i = 0; i < size; i++) {
    similarity_score += (long long)left[i] * freq[left[i]];
  }

  printf("Similarity Score: %lld\n", similarity_score);

  free(left);
  free(right);
  free(freq);
}
