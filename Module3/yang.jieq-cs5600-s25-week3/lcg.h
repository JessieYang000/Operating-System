#ifndef LCG_H
#define LCG_H

#include <stdio.h>
#include <stdlib.h>

// Constants for LCG
#define A 1664525
#define C 1013904223
#define M 4294967296
#define RANDOM_MIN 1
#define RANDOM_MAX 100

// Global variable for LCG state
extern unsigned long current_state;

// Function prototypes
void init_lcg(); // Initializes the seed
unsigned long lcg();
int genRand(int min, int max);

#endif // LCG_H