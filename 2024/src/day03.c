#include "day03.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MUL_PREFIX "mul("
#define MUL_PREFIX_LEN 4
#define DO_INSTRUCTION "do()"
#define DONT_INSTRUCTION "don't()"

const static char *FILE_NAME = "../inputs/day03.txt";

static bool is_valid_mul_char(const char c) { return isdigit(c) || c == ','; }

static bool parse_mul_numbers(const char *str, int *num1, int *num2) {
  return sscanf(str, "%d,%d", num1, num2) == 2;
}

static bool process_mul(const char *start, const char *end, int *result) {
  size_t nums_size = end - start + 1;
  char *nums = (char *)malloc(nums_size);
  if (!nums)
    return false;

  strncpy(nums, start, end - start);
  nums[end - start] = '\0';

  bool valid = true;
  for (char *p = nums; *p; p++) {
    if (!is_valid_mul_char(*p)) {
      valid = false;
      break;
    }
  }

  if (valid) {
    int num1, num2;
    if (parse_mul_numbers(nums, &num1, &num2)) {
      *result = num1 * num2;
      free(nums);
      return true;
    }
  }

  free(nums);
  return false;
}

static char *read_line(FILE *file) {
  char *line = (char *)malloc(BUFFER_SIZE);
  size_t capacity = BUFFER_SIZE;
  size_t length = 0;
  int c;

  while ((c = fgetc(file)) != EOF && c != '\n') {
    if (length + 1 >= capacity) {
      capacity *= 2;
      char *new_line = (char *)realloc(line, capacity);
      if (!new_line) {
        free(line);
        return NULL;
      }
      line = new_line;
    }
    line[length++] = c;
  }

  if (c == EOF && length == 0) {
    free(line);
    return NULL;
  }

  char *final_line = (char *)realloc(line, length + 1);
  if (!final_line) {
    free(line);
    return NULL;
  }
  final_line[length] = '\0';
  return final_line;
}

void day03_part1() {
  FILE *file = fopen(FILE_NAME, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", FILE_NAME);
    return;
  }

  int total = 0;
  char *line;

  while ((line = read_line(file))) {
    char *ptr = line;
    while ((ptr = strstr(ptr, MUL_PREFIX))) {
      char *start = ptr + MUL_PREFIX_LEN;
      char *end = strchr(start, ')');

      if (end) {
        int result;
        if (process_mul(start, end, &result)) {
          total += result;
        }
      }
      ptr++;
    }
    free(line);
  }

  printf("Part 1 Total: %d\n", total);
  fclose(file);
}

void day03_part2() {
  FILE *file = fopen(FILE_NAME, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", FILE_NAME);
    return;
  }

  int total = 0;
  bool mul_enabled = true;
  char *line;

  while ((line = read_line(file))) {
    char *ptr = line;
    while (*ptr) {
      if (strncmp(ptr, DO_INSTRUCTION, strlen(DO_INSTRUCTION)) == 0) {
        mul_enabled = true;
        ptr += strlen(DO_INSTRUCTION);
      } else if (strncmp(ptr, DONT_INSTRUCTION, strlen(DONT_INSTRUCTION)) ==
                 0) {
        mul_enabled = false;
        ptr += strlen(DONT_INSTRUCTION);
      } else if (strncmp(ptr, MUL_PREFIX, MUL_PREFIX_LEN) == 0 && mul_enabled) {
        char *start = ptr + MUL_PREFIX_LEN;
        char *end = strchr(start, ')');
        if (end) {
          int result;
          if (process_mul(start, end, &result)) {
            total += result;
          }
        }
        ptr++;
      } else {
        ptr++;
      }
    }
    free(line);
  }

  printf("Part 2 Total: %d\n", total);
  fclose(file);
}
