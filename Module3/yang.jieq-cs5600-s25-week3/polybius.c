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
