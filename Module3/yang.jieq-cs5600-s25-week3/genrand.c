#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

#define A 1664525
#define C 1013904223
#define M 4294967296
#define RANDOM_MIN 1
#define RANDOM_MAX 100

unsigned long current_state;

// Function of generating random numbers with algorithm Linear Congruential Generator
unsigned long lcg()
{
  current_state = (A * current_state + C) % M;
  return current_state;
}
// Function of generating random integers between min and max
int genRand(int min, int max)
{
  if (min > max)
  {
    fprintf(stderr, "Error: min should never be greater than max.\n");
    return -1;
  }

  return min + (lcg() % (max - min + 1));
}

int main(int argc, char *argv[])
{
  // Validate the number of arguments
  if (argc < 3 || argc > 4)
  {
    fprintf(stderr, "Usage: %s <number> <filename> [-a]\n", argv[0]);
    return 1;
  }

  // Initialize random seed ONCE in the entire program
  current_state = (unsigned long)time(NULL);

  // Validate the number of random numbers to generate
  char *endptr;

  // Convert input string to long integer using base 10, storing invalid characters' position in endptr
  long temp = strtol(argv[1], &endptr, 10);

  if (*endptr != '\0' || temp <= 0 || temp > INT_MAX)
  {
    fprintf(stderr, "Error: <number> must be a positive integer within the valid range.\n");
    return 1;
  }

  int num = (int)temp;
  const char *filename = argv[2];
  const char *mode = "w"; // Set the default mode to write

  // Set to append mode
  if (argc == 4 && strcmp(argv[3], "-a") == 0)
  {
    mode = "a";
  }

  FILE *file = fopen(filename, mode);
  if (file == NULL)
  {
    perror("Failed to open file");
    return 1;
  }

  printf("Writing %d random numbers to file %s.\n", num, filename);

  // Generate the required number of random integers and write to the given file
  for (int i = 0; i < num; i++)
  {
    // If the min and max are not valid, then terminate the loop and close the file
    if (fprintf(file, "%d\n", genRand(RANDOM_MIN, RANDOM_MAX)) < 0)
    {
      perror("Failed to write to file");
      fclose(file);
      return 1;
    };
  }

  fclose(file);
  printf("Random numbers successfully written to %s. \n", filename);
  return 0;
}
