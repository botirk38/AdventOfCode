#include "day07.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "../inputs/day07.txt"
#define MAX_LINE_LENGTH 1024

// Helper function prototypes
static long long concatenate(long long a, long long b);
static Equation *read_equations(const char *file_name, int *num_equations);
static long long evaluate(const long long *numbers, int count, const char *operators);
static int try_combinations(long long target, const long long *numbers, int count, char *operators, int pos);
static int try_combinations_part_2(long long target, const long long *numbers, int count, char *operators, int pos);
static void free_equations(Equation *equations, int num_equations);

static long long concatenate(long long a, long long b) {
    long long temp = b;
    long long multiplier = 1;
    
    while (temp > 0) {
        multiplier *= 10;
        temp /= 10;
    }
    
    return a * multiplier + b;
}

static Equation *read_equations(const char *file_name, int *num_equations) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", file_name);
        return NULL;
    }

    Equation *equations = NULL;
    *num_equations = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        Equation *new_eq = realloc(equations, (*num_equations + 1) * sizeof(Equation));
        if (!new_eq) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free_equations(equations, *num_equations);
            fclose(file);
            return NULL;
        }
        equations = new_eq;

        char *token = strtok(line, ":");
        if (!token) continue;

        equations[*num_equations].target = atoll(token);
        char *rest = strtok(NULL, "\n");
        if (!rest) continue;

        // Count numbers first
        int count = 0;
        char *temp_str = strdup(rest);
        if (!temp_str) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free_equations(equations, *num_equations);
            fclose(file);
            return NULL;
        }

        token = strtok(temp_str, " ");
        while (token) {
            if (*token) count++;
            token = strtok(NULL, " ");
        }
        free(temp_str);

        // Allocate and store numbers
        equations[*num_equations].numbers = malloc(count * sizeof(long long));
        if (!equations[*num_equations].numbers) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free_equations(equations, *num_equations);
            fclose(file);
            return NULL;
        }

        equations[*num_equations].count = count;
        token = strtok(rest, " ");
        for (int i = 0; i < count && token; i++) {
            if (*token) {
                equations[*num_equations].numbers[i] = atoll(token);
            }
            token = strtok(NULL, " ");
        }

        (*num_equations)++;
    }

    fclose(file);
    return equations;
}

static long long evaluate(const long long *numbers, int count, const char *operators) {
    long long result = numbers[0];
    
    for (int i = 1; i < count; i++) {
        switch (operators[i - 1]) {
            case '+':
                result += numbers[i];
                break;
            case '|':
                result = concatenate(result, numbers[i]);
                break;
            case '*':
                result *= numbers[i];
                break;
        }
    }
    
    return result;
}

static int try_combinations(long long target, const long long *numbers, int count,
                          char *operators, int pos) {
    if (pos == count - 1) {
        return evaluate(numbers, count, operators) == target;
    }

    const char ops[] = {'+', '*'};
    for (int i = 0; i < 2; i++) {
        operators[pos] = ops[i];
        if (try_combinations(target, numbers, count, operators, pos + 1)) {
            return 1;
        }
    }
    return 0;
}

static int try_combinations_part_2(long long target, const long long *numbers, int count,
                                 char *operators, int pos) {
    if (pos == count - 1) {
        return evaluate(numbers, count, operators) == target;
    }

    const char ops[] = {'+', '*', '|'};
    for (int i = 0; i < 3; i++) {
        operators[pos] = ops[i];
        if (try_combinations_part_2(target, numbers, count, operators, pos + 1)) {
            return 1;
        }
    }
    return 0;
}

static void free_equations(Equation *equations, int num_equations) {
    if (!equations) return;
    
    for (int i = 0; i < num_equations; i++) {
        free(equations[i].numbers);
    }
    free(equations);
}

void day07_part1(void) {
    int num_equations;
    Equation *equations = read_equations(FILE_NAME, &num_equations);
    if (!equations) {
        fprintf(stderr, "Error: Failed to read equations\n");
        return;
    }

    long long sum = 0;
    for (int i = 0; i < num_equations; i++) {
        if (equations[i].count > 1) {
            char *operators = calloc(equations[i].count - 1, sizeof(char));
            if (!operators) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                free_equations(equations, num_equations);
                return;
            }

            if (try_combinations(equations[i].target, equations[i].numbers,
                               equations[i].count, operators, 0)) {
                sum += equations[i].target;
            }
            free(operators);
        }
    }

    printf("Part 1 Sum: %lld\n", sum);
    free_equations(equations, num_equations);
}

void day07_part2(void) {
    int num_equations;
    Equation *equations = read_equations(FILE_NAME, &num_equations);
    if (!equations) {
        fprintf(stderr, "Error: Failed to read equations\n");
        return;
    }

    long long sum = 0;
    for (int i = 0; i < num_equations; i++) {
        if (equations[i].count > 1) {
            char *operators = calloc(equations[i].count - 1, sizeof(char));
            if (!operators) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                free_equations(equations, num_equations);
                return;
            }

            if (try_combinations_part_2(equations[i].target, equations[i].numbers,
                                      equations[i].count, operators, 0)) {
                sum += equations[i].target;
            }
            free(operators);
        }
    }

    printf("Part 2 Sum: %lld\n", sum);
    free_equations(equations, num_equations);
}

