#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *str;
} Pattern;

typedef struct {
  Pattern *patterns;
  int count;
  int capacity;
} PatternList;

typedef struct {
  PatternList *patterns;
  PatternList *designs;
} InputData;

// Part 1 memo structure
typedef struct {
  char *key;
  bool result;
} BoolMemoEntry;

typedef struct {
  BoolMemoEntry *entries;
  int count;
  int capacity;
} BoolMemoTable;

// Part 2 memo structure
typedef struct {
  char *key;
  long result;
} LongMemoEntry;

typedef struct {
  LongMemoEntry *entries;
  int count;
  int capacity;
} LongMemoTable;

static PatternList *create_pattern_list(int initial_capacity) {
  PatternList *list = malloc(sizeof(PatternList));
  list->patterns = malloc(sizeof(Pattern) * initial_capacity);
  list->count = 0;
  list->capacity = initial_capacity;
  return list;
}

static void add_pattern(PatternList *list, const char *str) {
  if (list->count == list->capacity) {
    list->capacity *= 2;
    list->patterns = realloc(list->patterns, sizeof(Pattern) * list->capacity);
  }
  list->patterns[list->count].str = strdup(str);
  list->count++;
}

static BoolMemoTable *create_bool_memo_table(int capacity) {
  BoolMemoTable *table = malloc(sizeof(BoolMemoTable));
  table->entries = malloc(sizeof(BoolMemoEntry) * capacity);
  table->count = 0;
  table->capacity = capacity;
  return table;
}

static LongMemoTable *create_long_memo_table(int capacity) {
  LongMemoTable *table = malloc(sizeof(LongMemoTable));
  table->entries = malloc(sizeof(LongMemoEntry) * capacity);
  table->count = 0;
  table->capacity = capacity;
  return table;
}

static bool bool_memo_lookup(BoolMemoTable *table, const char *key,
                             bool *result) {
  for (int i = 0; i < table->count; i++) {
    if (strcmp(table->entries[i].key, key) == 0) {
      *result = table->entries[i].result;
      return true;
    }
  }
  return false;
}

static bool long_memo_lookup(LongMemoTable *table, const char *key,
                             long *result) {
  for (int i = 0; i < table->count; i++) {
    if (strcmp(table->entries[i].key, key) == 0) {
      *result = table->entries[i].result;
      return true;
    }
  }
  return false;
}

static void bool_memo_add(BoolMemoTable *table, const char *key, bool result) {
  if (table->count == table->capacity) {
    table->capacity *= 2;
    table->entries =
        realloc(table->entries, sizeof(BoolMemoEntry) * table->capacity);
  }
  table->entries[table->count].key = strdup(key);
  table->entries[table->count].result = result;
  table->count++;
}

static void long_memo_add(LongMemoTable *table, const char *key, long result) {
  if (table->count == table->capacity) {
    table->capacity *= 2;
    table->entries =
        realloc(table->entries, sizeof(LongMemoEntry) * table->capacity);
  }
  table->entries[table->count].key = strdup(key);
  table->entries[table->count].result = result;
  table->count++;
}

static bool can_make_design_memo(const char *design, PatternList *patterns,
                                 BoolMemoTable *memo) {
  if (strlen(design) == 0)
    return true;

  bool cached_result;
  if (bool_memo_lookup(memo, design, &cached_result)) {
    return cached_result;
  }

  for (int i = 0; i < patterns->count; i++) {
    const char *pattern = patterns->patterns[i].str;
    size_t pattern_len = strlen(pattern);

    if (pattern_len <= strlen(design) &&
        strncmp(design, pattern, pattern_len) == 0) {
      if (can_make_design_memo(design + pattern_len, patterns, memo)) {
        bool_memo_add(memo, design, true);
        return true;
      }
    }
  }

  bool_memo_add(memo, design, false);
  return false;
}

static long count_ways_memo(const char *design, PatternList *patterns,
                            LongMemoTable *memo) {
  if (strlen(design) == 0) {
    return 1;
  }

  long cached_result;
  if (long_memo_lookup(memo, design, &cached_result)) {
    return cached_result;
  }

  long total = 0;
  for (int i = 0; i < patterns->count; i++) {
    const char *pattern = patterns->patterns[i].str;
    size_t pattern_len = strlen(pattern);

    if (pattern_len <= strlen(design) &&
        strncmp(design, pattern, pattern_len) == 0) {
      total += count_ways_memo(design + pattern_len, patterns, memo);
    }
  }

  long_memo_add(memo, design, total);
  return total;
}

static InputData *load_input(const char *filename) {
  FILE *file = fopen(filename, "r");
  InputData *data = malloc(sizeof(InputData));
  data->patterns = create_pattern_list(10);
  data->designs = create_pattern_list(10);

  char *line = NULL;
  size_t len = 0;

  getline(&line, &len, file);
  if (line[strlen(line) - 1] == '\n') {
    line[strlen(line) - 1] = '\0';
  }

  char *token = strtok(line, ", ");
  while (token != NULL) {
    while (*token == ' ')
      token++;
    add_pattern(data->patterns, token);
    token = strtok(NULL, ", ");
  }

  free(line);
  line = NULL;

  getline(&line, &len, file);
  free(line);
  line = NULL;

  while (getline(&line, &len, file) != -1) {
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    if (strlen(line) > 0) {
      add_pattern(data->designs, line);
    }
    free(line);
    line = NULL;
  }

  fclose(file);
  return data;
}

static void free_bool_memo_table(BoolMemoTable *table) {
  for (int i = 0; i < table->count; i++) {
    free(table->entries[i].key);
  }
  free(table->entries);
  free(table);
}

static void free_long_memo_table(LongMemoTable *table) {
  for (int i = 0; i < table->count; i++) {
    free(table->entries[i].key);
  }
  free(table->entries);
  free(table);
}

static void free_pattern_list(PatternList *list) {
  for (int i = 0; i < list->count; i++) {
    free(list->patterns[i].str);
  }
  free(list->patterns);
  free(list);
}

void day19_part1(void) {
  InputData *data = load_input("../inputs/day19.txt");
  BoolMemoTable *memo = create_bool_memo_table(1000);

  int possible_count = 0;
  for (int i = 0; i < data->designs->count; i++) {
    if (can_make_design_memo(data->designs->patterns[i].str, data->patterns,
                             memo)) {
      possible_count++;
    }
  }

  printf("Part 1 Result: %d\n", possible_count);

  free_bool_memo_table(memo);
  free_pattern_list(data->patterns);
  free_pattern_list(data->designs);
}

void day19_part2(void) {
  InputData *data = load_input("../inputs/day19.txt");
  LongMemoTable *memo = create_long_memo_table(1000);

  long total_ways = 0;
  for (int i = 0; i < data->designs->count; i++) {
    total_ways +=
        count_ways_memo(data->designs->patterns[i].str, data->patterns, memo);
  }

  printf("Part 2 Result: %ld\n", total_ways);

  free_long_memo_table(memo);
  free_pattern_list(data->patterns);
  free_pattern_list(data->designs);
}

