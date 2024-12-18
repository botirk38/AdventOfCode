#include "day17.h"
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 8
#define CHUNK_SIZE (1LL << 25)

typedef struct {
  int A;
  int B;
  int C;
  int ip;
} Computer;

typedef struct {
  int *values;
  int count;
  int capacity;
} Output;

typedef struct {
  int A;
  int B;
  int C;
} Registers;

typedef struct {
  int *codes;
  int size;
} Program;

typedef struct {
  Registers regs;
  Program prog;
} ProgramData;

static void init_output(Output *out) {
  out->capacity = 16;
  out->values = malloc(out->capacity * sizeof(int));
  out->count = 0;
}

static void add_output(Output *out, int value) {
  if (out->count >= out->capacity) {
    out->capacity *= 2;
    out->values = realloc(out->values, out->capacity * sizeof(int));
  }
  out->values[out->count++] = value;
}

static int get_combo_value(Computer *cpu, int operand) {
  switch (operand) {
  case 0:
  case 1:
  case 2:
  case 3:
    return operand;
  case 4:
    return cpu->A;
  case 5:
    return cpu->B;
  case 6:
    return cpu->C;
  default:
    return 0;
  }
}

static void execute_instruction(Computer *cpu, int opcode, int operand,
                                Output *out) {
  switch (opcode) {
  case 0: // adv
    cpu->A = cpu->A / (1 << get_combo_value(cpu, operand));
    break;
  case 1: // bxl
    cpu->B ^= operand;
    break;
  case 2: // bst
    cpu->B = get_combo_value(cpu, operand) % 8;
    break;
  case 3: // jnz
    if (cpu->A != 0) {
      cpu->ip = operand - 2;
    }
    break;
  case 4: // bxc
    cpu->B ^= cpu->C;
    break;
  case 5: // out
    add_output(out, get_combo_value(cpu, operand) % 8);
    break;
  case 6: // bdv
    cpu->B = cpu->A / (1 << get_combo_value(cpu, operand));
    break;
  case 7: // cdv
    cpu->C = cpu->A / (1 << get_combo_value(cpu, operand));
    break;
  }
}

static char *read_line(FILE *file) {
  size_t capacity = 128;
  char *line = malloc(capacity);
  size_t length = 0;
  int c;

  while ((c = fgetc(file)) != EOF && c != '\n') {
    if (length + 1 >= capacity) {
      capacity *= 2;
      line = realloc(line, capacity);
    }
    line[length++] = c;
  }

  if (c == EOF && length == 0) {
    free(line);
    return NULL;
  }

  line[length] = '\0';
  return line;
}

static ProgramData *read_program(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file: %s\n", filename);
    exit(1);
  }

  ProgramData *data = malloc(sizeof(ProgramData));
  data->prog.codes = malloc(16 * sizeof(int));
  data->prog.size = 0;
  int capacity = 16;
  bool reading_program = false;

  char *line;
  while ((line = read_line(file)) != NULL) {
    if (strlen(line) == 0) {
      free(line);
      continue;
    }

    if (!reading_program) {
      if (strstr(line, "Register A:")) {
        sscanf(line, "Register A: %d", &data->regs.A);
      } else if (strstr(line, "Register B:")) {
        sscanf(line, "Register B: %d", &data->regs.B);
      } else if (strstr(line, "Register C:")) {
        sscanf(line, "Register C: %d", &data->regs.C);
      } else if (strstr(line, "Program:")) {
        reading_program = true;
        char *program_str = line + 8;
        while (*program_str == ' ')
          program_str++;

        char *token = strtok(program_str, ",");
        while (token) {
          if (data->prog.size >= capacity) {
            capacity *= 2;
            data->prog.codes =
                realloc(data->prog.codes, capacity * sizeof(int));
          }
          data->prog.codes[data->prog.size++] = atoi(token);
          token = strtok(NULL, ",");
        }
      }
    }
    free(line);
  }

  fclose(file);
  return data;
}

static void run_program(Computer *cpu, const int *program, int program_start,
                        int length, Output *out) {
  cpu->ip = program_start;

  while (cpu->ip < length) {
    int opcode = program[cpu->ip];
    int operand = program[cpu->ip + 1];
    execute_instruction(cpu, opcode, operand, out);
    cpu->ip += 2;
  }
}

void day17_part1(void) {
  ProgramData *data = read_program("../inputs/day17.txt");

  Computer cpu = {
      .A = data->regs.A, .B = data->regs.B, .C = data->regs.C, .ip = 0};

  Output out;
  init_output(&out);

  run_program(&cpu, data->prog.codes, 0, data->prog.size, &out);

  for (int i = 0; i < out.count; i++) {
    printf("%d%s", out.values[i], i < out.count - 1 ? "," : "");
  }
  printf("\n");

  free(data->prog.codes);
  free(data);
  free(out.values);
}

static void run_with_candidate(Computer *cpu, const int *program, int length,
                               Output *out) {
  run_program(cpu, program, 0, length, out);
}

int64_t find_self_replicating_value(int *program, int program_length) {
  int64_t a = 0;

  for (int i = program_length - 1; i >= 0; i--) {
    a <<= 3;

    for (int64_t b = 0; b < 8; b++) {
      int64_t candidate = a + b;

      Computer c = {.A = (int)candidate, .B = 0, .C = 0, .ip = 0};

      Output out;
      init_output(&out);

      run_with_candidate(&c, program, program_length, &out);

      bool matches = true;
      for (int j = 0; j < program_length - i && j < out.count; j++) {
        if (out.values[j] != program[i + j]) {
          matches = false;
          break;
        }
      }

      if (matches && out.count >= program_length - i) {
        a = candidate;
        free(out.values);
        break;
      }

      free(out.values);
    }
  }

  return a;
}

void day17_part2(void) {
  ProgramData *data = read_program("../inputs/day17.txt");
  int64_t result =
      find_self_replicating_value(data->prog.codes, data->prog.size);
  printf("Part 2: %lld\n", result);

  free(data->prog.codes);
  free(data);
}
