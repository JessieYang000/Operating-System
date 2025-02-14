#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

/* A function to generate random strings*/
char *genString(int length)
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    char *randomStr = malloc(length + 1);
    if (randomStr == NULL)
    {
        perror("Memory allocation failed.");
        return NULL;
    }

    for (int i = 0; i < length; i++)
    {
        randomStr[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    randomStr[length] = '\0'; // Ensure strings are properly terminated
    return randomStr;
}

/* A function to create a file and write strings into it*/
int saveStringsInFile(char *filename, int num, int length)
{
    // Set the append mode
    const char *mode = "a"; // Use append mode instead of overwrite

    // Create a file with given name and append mode
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    // Write randome strings into the file
    printf("Writing %d random strings with length %d to %s...\n", num, length, filename);
    for (int i = 0; i < num; i++)
    {
        char *randomString = genString(length); // Generate a random string
        if (randomString == NULL)
        {
            perror("Memory allocation failed.");
            fclose(file);
            return 1;
        }

        // Write the generated string into the file
        if (fprintf(file, "%s\n", randomString) < 0)
        {
            perror("Failed to write to file");
            free(randomString);
            fclose(file);
            return 1;
        }
        free(randomString);
    }

    fclose(file);
    printf("Random strings successfully written to %s.\n", filename);

    return 0;
}

int read_file_to_queue(const char *filename, queue_t *queue, int length)
{
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    char *buffer = malloc(length + 2); // Buffer to hold each line
    if (!buffer)
    {
        perror("Memory allocation failed.");
        fclose(file);
        return 1;
    }

    printf("Saving strings from file %s into queue\n", filename);

    while (fgets(buffer, sizeof(buffer), file))
    {
        // Remove trailing newline character, if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len == 0)
            continue;

        // Allocate memory for the string and copy the line
        char *line = malloc(len + 1);
        if (line == NULL)
        {
            perror("Memory allocation failed");
            fclose(file);
            return 1;
        }
        strcpy(line, buffer);

        // Push the dynamically allocated string to the queue
        push_queue(queue, line);
    }

    printf("%d strings saved in queue\n", queue->size);

    fclose(file); // Close the file
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Invalid arguments. Usage: %s <number> <string length> <filename>\n", argv[0]);
        return 1;
    }

    srand(time(NULL)); // Seed the random number generator with the current time to ensure different random values on each run

    // Retrieve the number of strings and the length of each string
    char *endptr;
    long temp1 = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || temp1 < 10000)
    {
        fprintf(stderr, "Error: <number> must be a positive integer at least 10,000.\n");
        return 1;
    }

    long temp2 = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || temp2 <= 0)
    {
        fprintf(stderr, "Error: <length> must be a positive integer. \n");
        return 1;
    }

    int num = (int)temp1;
    int length = (int)temp2;

    // Retrieve the filename
    char *filename = argv[3];

    // Create a file and save strings in it
    if (saveStringsInFile(filename, num, length) != 0)
        return 1;

    queue_t *queue = malloc(sizeof(queue_t));
    if (!queue)
    {
        perror("Failed to allocate memory for queue.");
        return 1;
    }

    init_queue(queue);

    if (read_file_to_queue(filename, queue, length) != 0)
        return 1;

    return 0;
}
