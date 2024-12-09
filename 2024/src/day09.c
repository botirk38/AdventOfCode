#include "day09.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "../inputs/day09.txt"

char* read_input_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    
    char* input = malloc(size + 1);
    fread(input, 1, size, fp);
    input[size] = '\0';
    fclose(fp);
    
    return input;
}

Disk* uncompress(const char* input) {
    Disk* disk = malloc(sizeof(Disk));
    int total_len = 0;
    
    // Calculate total length first
    for (int i = 0; input[i] && input[i] != '\n'; i++) {
        total_len += input[i] - '0';
    }
    
    disk->length = total_len;
    disk->blocks = malloc(total_len * sizeof(int));
    
    int pos = 0;
    int id = 0;
    
    for (int i = 0; input[i] && input[i] != '\n'; i++) {
        int count = input[i] - '0';
        if (i % 2 == 0) {
            // File blocks
            for (int j = 0; j < count; j++) {
                disk->blocks[pos++] = id;
            }
            id++;
        } else {
            // Free space
            for (int j = 0; j < count; j++) {
                disk->blocks[pos++] = -1;
            }
        }
    }
    
    return disk;
}

void move_to_start(Disk* disk) {
    int left = 0;
    int right = disk->length - 1;
    
    while (left < right) {
        // Skip non-empty blocks from left
        while (left < right && disk->blocks[left] != -1) {
            left++;
        }
        
        // Skip empty blocks from right
        while (left < right && disk->blocks[right] == -1) {
            right--;
        }
        
        if (left < right) {
            // Swap blocks
            disk->blocks[left] = disk->blocks[right];
            disk->blocks[right] = -1;
            left++;
            right--;
        }
    }
}

long calculate_checksum(Disk* disk) {
    long sum = 0;
    for (int i = 0; i < disk->length; i++) {
        if (disk->blocks[i] != -1) {
            sum += (long)i * disk->blocks[i];
        }
    }
    return sum;
}

void move_files_to_start(Disk* disk) {
    for (int file_id = (disk->length - 1) / 2; file_id >= 0; file_id--) {
        int file_length = 0;
        int file_start = -1;
        
        for (int i = 0; i < disk->length; i++) {
            if (disk->blocks[i] == file_id) {
                if (file_start == -1) file_start = i;
                file_length++;
            }
        }
        
        if (file_length == 0) continue;  // File not found
        
        int free_start = -1;
        int free_length = 0;
        
        for (int i = 0; i < file_start; i++) {
            if (disk->blocks[i] == -1) {
                if (free_start == -1) free_start = i;
                free_length++;
            } else {
                free_start = -1;
                free_length = 0;
            }
            
            if (free_length == file_length) break;
        }
        
        if (free_start != -1 && free_length >= file_length) {
            // Move the whole file
            for (int i = 0; i < file_length; i++) {
                disk->blocks[free_start + i] = file_id;
                disk->blocks[file_start + i] = -1;
            }
        }
    }
}

void day09_part1(void) {
   char* input = read_input_file(FILE_NAME);
    if (!input) {
        return;
    } 
    Disk* disk = uncompress(input);
    move_to_start(disk);
    long checksum = calculate_checksum(disk);
    
    printf("Checksum: %ld\n", checksum);
    
    free(input);
    free(disk->blocks);
    free(disk);
}



void day09_part2(void) {
      char* input = read_input_file(FILE_NAME);
    if (!input) {
        return;
    } 
 
    Disk* disk = uncompress(input);
    move_files_to_start(disk);
    long checksum = calculate_checksum(disk);
    
    printf("Part 2 Checksum: %ld\n", checksum);
    
    free(input);
    free(disk->blocks);
    free(disk);
}

