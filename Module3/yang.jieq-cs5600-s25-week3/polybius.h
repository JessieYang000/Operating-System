#ifndef POLYBIUS_H
#define POLYBIUS_H

#define ROWS 6
#define COLS 5

// Define a structure for the Polybius table
typedef struct {
    char grid[ROWS][COLS];  // The 6×5 table
} PolybiusTable_t;

// Function prototypes
void pbEncode(const char *plaintext, PolybiusTable_t table);
void pbDecode(const char *ciphertext, PolybiusTable_t table);

#endif  // POLYBIUS_H
