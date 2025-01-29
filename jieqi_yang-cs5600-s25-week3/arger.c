#include <stdio.h>

// Function to compare two strings (manual implementation of strcmp)
int string_equals(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) {
            return 0;  // Strings are not equal
        }
        s1++;
        s2++;
    }
    return *s1 == '\0' && *s2 == '\0';  // Return true if both strings are null-terminated
}

// Function to convert a string to uppercase
void to_uppercase(char *str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32;  // Convert lowercase to uppercase
        }
        str++;
    }
}

// Function to convert a string to lowercase
void to_lowercase(char *str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 32;  // Convert uppercase to lowercase
        }
        str++;
    }
}

// Function to convert a string to title case
void to_titlecase(char *str) {
    int capitalize = 1;  // Flag to determine if the next character should be capitalized
    while (*str) {
        if (*str == ' ') {
            capitalize = 1;  // Next letter should be capitalized after a space
        } else if (capitalize && *str >= 'a' && *str <= 'z') {
            *str -= 32;  // Capitalize the first letter
            capitalize = 0;
        } else if (*str >= 'A' && *str <= 'Z') {
            *str += 32;  // Convert the rest to lowercase
        }
        str++;
    }
}

int main(int argc, char *argv[]) {
    // Check if there are enough arguments
    if (argc < 2) {
        fprintf(stderr, "Error: Not enough arguments provided.\n");
        fprintf(stderr, "Usage: %s [-u | -cap | -l] <text>\n", argv[0]);
        return -1;
    }

    int uppercase = 0;  // Flag for uppercase conversion
    int titlecase = 0;  // Flag for title case conversion
    int lowercase = 0;  // Flag for lowercase conversion
    int start_index = 1;

    // Validate the first argument
    if (string_equals(argv[1], "-u")) {
        uppercase = 1;
        start_index = 2;  // Skip the "-u" flag
    } else if (string_equals(argv[1], "-cap")) {
        titlecase = 1;
        start_index = 2;  // Skip the "-cap" flag
    } else if (string_equals(argv[1], "-l")) {
        lowercase = 1;
        start_index = 2;  // Skip the "-l" flag
    } else {
        fprintf(stderr, "Error: Invalid flag '%s'. Use '-u', '-cap', or '-l'.\n", argv[1]);
        return -1;
    }

    // Check if there are arguments to process
    if (start_index >= argc) {
        fprintf(stderr, "Error: No text provided to process.\n");
        return -1;
    }

    // Process and display the arguments
    for (int i = start_index; i < argc; i++) {
        char *word = argv[i];
        if (uppercase) {
            to_uppercase(word);
        } else if (titlecase) {
            to_titlecase(word);
        } else if (lowercase) {
            to_lowercase(word);
        }
        printf("%s ", word);
    }
    printf("\n");

    return 0;  // Exit with success
}

