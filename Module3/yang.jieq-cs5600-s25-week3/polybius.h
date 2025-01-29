#ifndef POLYBIUS_H
#define POLYBIUS_H

#define ROWS 6
#define COLS 5

// Define a structure for the Polybius table
typedef struct {
    char grid[ROWS][COLS];  // The 6×5 table
} PolybiusTable;

// Function prototypes
void pbEncode(const char *plaintext, PolybiusTable table);
void pbDecode(const char *ciphertext, PolybiusTable table);

#endif  // POLYBIUS_H
