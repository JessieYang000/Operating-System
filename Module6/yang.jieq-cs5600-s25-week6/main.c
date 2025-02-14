#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "strings.h"
#include <time.h>

#define MAX_LENGTH 1024
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num_strings> <string_length> <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Retrieve the number of strings and the length of each string
    char *endptr;
    long temp1 = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || temp1 < 10000)
    {
        fprintf(stderr, "Error: <number> must be a positive integer at least 10,000.\n");
        return 1;
    }

    long temp2 = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || temp2 <= 0 || temp2 > MAX_LENGTH)
    {
        fprintf(stderr, "Error: <length> must be between 1 and %d.\n", MAX_LENGTH);
        return 1;
    }

    int num = (int)temp1;
    int length = (int)temp2;

    char *filename = argv[3];

    queue_t queue;
    init_queue(&queue);

    srand(time(NULL)); // Seed the random number generator with the current time to ensure different random values on each run

    // Task 1: Generate and save strings to file
    generate_and_save_strings(num, length, filename);
    

    // Task 2: Load strings into queue
    save_in_queue(filename, &queue);

    // // Task 3: Process queue with cipher
    // process_queue_with_cipher(&queue);

    return 0;
}
