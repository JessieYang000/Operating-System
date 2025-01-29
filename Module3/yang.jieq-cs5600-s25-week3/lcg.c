#include "lcg.h"
#include <time.h>

// Global state variable
unsigned long current_state;

// Initialize random seed
void init_lcg() {
    current_state = (unsigned long)time(NULL);
}

// Linear Congruential Generator
unsigned long lcg() {
    current_state = (A * current_state + C) % M;
    return current_state;
}

// Generates a random integer in a given range
int genRand(int min, int max) {
    if (min > max) {
        fprintf(stderr, "Error: min should never be greater than max.\n");
        return -1;
    }
    return min + (lcg() % (max - min + 1));
}
