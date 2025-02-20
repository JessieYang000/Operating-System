#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "strings.h"

/* A function to generate random strings*/
char *genString(int length)
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
    const char *mode = "w"; // Use write mode instead of appending

    // Create a file with given name
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
        /*
        Generate a random string(Could consider declare randomString out of for-loop and reuse it, so only need to free it once at the end of the method. If in this case, we need to pass it as the second variable with the variable length into genString() method).
        */
        char *randomString = genString(length); 
        if (randomString == NULL)
        {
            perror("Memory allocation failed.");
            fclose(file);
            return 1;
        }

        // Write the generated string into the file
        if (fprintf(file, "%s\n", randomString) < 0) // fprintf() returns  the number of characters successfully written to the specified output stream
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

/* Generate random strings and save them to a file */
void generate_and_save_strings(int num, int length, char *filename) {
    // Ensure valid input values
    if (num <= 0 || length <= 0) {
        fprintf(stderr, "Error: Number of strings and length must be positive.\n");
        return;
    }

    if (saveStringsInFile(filename, num, length) != 0) {
        fprintf(stderr, "Error: Failed to generate and save strings.\n");
    }
}
