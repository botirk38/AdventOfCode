#include "day06.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static char *FILE_NAME = "../inputs/day06.txt";

#define BUFFER_SIZE 1024

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

static void allocate_lab(Lab *lab, int rows, int cols) {
  lab->rows = rows;
  lab->cols = cols;

  // Allocate map
  lab->map = malloc(rows * sizeof(char *));
  lab->visited = malloc(rows * sizeof(bool *));
  for (int i = 0; i < rows; i++) {
    lab->map[i] = malloc(cols * sizeof(char));
    lab->visited[i] = calloc(cols, sizeof(bool)); // Initialize to false
  }
  lab->guard = malloc(sizeof(Guard));
}

static void find_guard(Lab *lab) {

  for (int i = 0; i < lab->rows; i++) {

    for (int j = 0; j < lab->cols; j++) {

      if (lab->map[i][j] == '^') {
        lab->guard->x = j;
        lab->guard->y = i;
        lab->guard->direction = '^';
      }
    }
  }
}

static void turn_right(Guard *guard) {
  switch (guard->direction) {
  case '^':
    guard->direction = '>';
    break;
  case '>':
    guard->direction = 'v';
    break;
  case 'v':
    guard->direction = '<';
    break;
  case '<':
    guard->direction = '^';
    break;
  }
}

static bool is_obstacle(Lab *lab, int x, int y) {
  if (x < 0 || x >= lab->cols || y < 0 || y >= lab->rows)
    return true;
  return lab->map[y][x] == '#';
}

static bool move_guard(Lab *lab) {
  int next_x = lab->guard->x;
  int next_y = lab->guard->y;

  // Check front position based on direction
  switch (lab->guard->direction) {
  case '^':
    next_y--;
    break;
  case '>':
    next_x++;
    break;
  case 'v':
    next_y++;
    break;
  case '<':
    next_x--;
    break;
  }

  // If position is outside map, return false
  if (next_x < 0 || next_x >= lab->cols || next_y < 0 || next_y >= lab->rows) {
    return false;
  }

  // If obstacle ahead, turn right
  if (lab->map[next_y][next_x] == '#') {
    turn_right(lab->guard);
    return true;
  }

  // Move forward
  lab->guard->x = next_x;
  lab->guard->y = next_y;
  return true;
}

static void free_lab(Lab *lab) {
  for (int i = 0; i < lab->rows; i++) {
    free(lab->map[i]);
    free(lab->visited[i]);
  }
  free(lab->map);
  free(lab->visited);
  free(lab->guard);
}

static void print_lab_state(Lab *lab) {
  printf("\nCurrent Lab State:\n");
  printf("Guard position: (%d,%d) facing %c\n", lab->guard->x, lab->guard->y,
         lab->guard->direction);

  for (int i = 0; i < lab->rows; i++) {
    for (int j = 0; j < lab->cols; j++) {
      if (i == lab->guard->y && j == lab->guard->x) {
        printf("%c", lab->guard->direction);
      } else {
        printf("%c", lab->map[i][j]);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void day06_part1() {

  FILE *file = fopen(FILE_NAME, "r");

  if (!file) {
    printf("File failed to open:\n");
  }

  char *line;

  int rows = 0;
  int cols = 0;

  while ((line = read_line(file))) {
    rows++;
    printf("Line find dims: %s\n", line);

    if (strlen(line) > cols) {
      cols = strlen(line);
    }

    free(line);
  }

  Lab lab;

  allocate_lab(&lab, rows, cols);

  // Read data into puzzle map

  rewind(file);

  int row = 0;

  while ((line = read_line(file))) {

    printf("Line reading data: %s\n", line);

    strcpy(lab.map[row], line);
    row++;
    free(line);
  }

  fclose(file);

  // Find the guard

  find_guard(&lab);

  lab.visited[lab.guard->y][lab.guard->x] = true;
  int visited_count = 1;

  while (move_guard(&lab)) {

    if (!lab.visited[lab.guard->y][lab.guard->x]) {
      lab.visited[lab.guard->y][lab.guard->x] = true;
      visited_count++;
    }
  }

  printf("Visited Count:%d\n", visited_count);

  free_lab(&lab);
}

static bool detect_loop(Lab *lab) {
  typedef struct {
    bool up, right, down, left;
  } DirectionState;

  // Allocate visited states array
  DirectionState **visited = malloc(lab->rows * sizeof(DirectionState *));
  for (int i = 0; i < lab->rows; i++) {
    visited[i] = calloc(lab->cols, sizeof(DirectionState));
  }

  int steps = 0;

  // Mark initial state
  switch (lab->guard->direction) {
  case '^':
    visited[lab->guard->y][lab->guard->x].up = true;
    break;
  case '>':
    visited[lab->guard->y][lab->guard->x].right = true;
    break;
  case 'v':
    visited[lab->guard->y][lab->guard->x].down = true;
    break;
  case '<':
    visited[lab->guard->y][lab->guard->x].left = true;
    break;
  }

  while (true) {
    if (!move_guard(lab)) {
      // Guard left the map
      for (int i = 0; i < lab->rows; i++)
        free(visited[i]);
      free(visited);
      return false;
    }

    // Check current state
    DirectionState *current = &visited[lab->guard->y][lab->guard->x];
    bool already_visited = false;

    switch (lab->guard->direction) {
    case '^':
      if (current->up)
        already_visited = true;
      current->up = true;
      break;
    case '>':
      if (current->right)
        already_visited = true;
      current->right = true;
      break;
    case 'v':
      if (current->down)
        already_visited = true;
      current->down = true;
      break;
    case '<':
      if (current->left)
        already_visited = true;
      current->left = true;
      break;
    }

    if (already_visited) {
      // We've been here in this direction before - it's a loop
      for (int i = 0; i < lab->rows; i++)
        free(visited[i]);
      free(visited);
      return true;
    }

    steps++;
  }

  for (int i = 0; i < lab->rows; i++)
    free(visited[i]);
  free(visited);
  return false;
}

static bool test_obstacle_position(Lab *lab, int x, int y) {
  // Save original state
  char original = lab->map[y][x];
  int orig_guard_x = lab->guard->x;
  int orig_guard_y = lab->guard->y;
  char orig_guard_dir = lab->guard->direction;

  // Place obstacle
  lab->map[y][x] = '#';

  // Test for loop
  bool creates_loop = detect_loop(lab);

  // Restore original state
  lab->map[y][x] = original;
  lab->guard->x = orig_guard_x;
  lab->guard->y = orig_guard_y;
  lab->guard->direction = orig_guard_dir;

  return creates_loop;
}

void day06_part2() {

  FILE *file = fopen(FILE_NAME, "r");

  if (!file) {
    printf("File failed to open:\n");
  }

  char *line;

  int rows = 0;
  int cols = 0;

  while ((line = read_line(file))) {
    rows++;
    printf("Line find dims: %s\n", line);

    if (strlen(line) > cols) {
      cols = strlen(line);
    }

    free(line);
  }

  Lab lab;

  allocate_lab(&lab, rows, cols);

  // Read data into puzzle map

  rewind(file);

  int row = 0;

  while ((line = read_line(file))) {

    printf("Line reading data: %s\n", line);

    strcpy(lab.map[row], line);
    row++;
    free(line);
  }

  fclose(file);

  // Find the guard

  find_guard(&lab);

  int valid_positions = 0;

  for (int y = 0; y < lab.rows; y++) {
    for (int x = 0; x < lab.cols; x++) {
      // Skip if not empty or guard's starting position
      if (lab.map[y][x] != '.' || (x == lab.guard->x && y == lab.guard->y)) {
        continue;
      }

      if (test_obstacle_position(&lab, x, y)) {
        valid_positions++;
      }
    }
  }

  printf("Number of valid obstacle positions: %d\n", valid_positions);
  free_lab(&lab);
}
