#ifndef DAY07_H
#define DAY07_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    long long target;    
    long long *numbers;  
    int count;         
} Equation;

// Function declarations
void day07_part1(void);
void day07_part2(void);

#ifdef __cplusplus
}
#endif

#endif /* DAY07_H */

