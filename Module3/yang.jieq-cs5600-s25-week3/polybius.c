#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "polybius.h"

// Function to encode plaintext into Polybius coordinates
void pbEncode(const char *plaintext, PolybiusTable table) {
    printf("Encoded: ");
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char c = toupper(plaintext[i]);  // Convert to uppercase

        // Ignore spaces but keep them in the output
        if (c == ' ') {
            printf("  ");
            continue;
        }

        // Find the character in the Polybius Square
        int found = 0;
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (table.grid[row][col] == c) {
                    printf("%d%d ", row + 1, col + 1);
                    found = 1;
                    break;
                }
            }
            if (found) break;
        }

        // If character is not found, print it unchanged (for special characters)
        if (!found) {
            printf("%c ", c);
        }
    }
    printf("\n");
}

// Function to decode a Polybius ciphertext back to text
void pbDecode(const char *ciphertext, PolybiusTable table) {
    printf("Decoded: ");
    for (int i = 0; i < strlen(ciphertext); i++) {
        if (!isdigit(ciphertext[i])) {  
            printf("%c", ciphertext[i]); // Print special characters unchanged
            continue;
        }

        // Convert two-digit number into (row, col)
        int row = ciphertext[i] - '0' - 1;
        int col = ciphertext[i + 1] - '0' - 1;

        // Ensure valid coordinates before printing
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            printf("%c", table.grid[row][col]);
        }

        i++;  // Skip next index
    }
    printf("\n");
}
