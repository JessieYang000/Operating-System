#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "lcg.h"

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <number> <filename> [-a]\n", argv[0]);
        return 1;
    }

    init_lcg();  // Initialize the LCG seed

    char *endptr;
    long temp = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || temp <= 0 || temp > INT_MAX) {
        fprintf(stderr, "Error: <number> must be a positive integer.\n");
        return 1;
    }
    int num = (int)temp;

    const char *filename = argv[2];
    const char *mode = "w";  
    if (argc == 4 && strcmp(argv[3], "-a") == 0) {
        mode = "a";
    }

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    printf("Writing %d random numbers to %s...\n", num, filename);
    for (int i = 0; i < num; i++) {
        if (fprintf(file, "%d\n", genRand(RANDOM_MIN, RANDOM_MAX)) < 0) {
            perror("Failed to write to file");
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    printf("Random numbers successfully written to %s.\n", filename);
    return 0;
}
