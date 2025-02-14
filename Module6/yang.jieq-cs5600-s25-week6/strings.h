#ifndef STRINGS_H
#define STRINGS_H

/* Function to generate a random string of specified length */
char *genString(int length);

/* Function to save a given number of random strings to a file */
int saveStringsInFile(char *filename, int num, int length);

/* Function to generate random strings and save them */
void generate_and_save_strings(int num, int length, char *filename);

#endif  // STRINGS_H
