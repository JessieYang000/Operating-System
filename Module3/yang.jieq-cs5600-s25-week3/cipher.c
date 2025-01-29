#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "polybius.h"

// Entry point: Handles encryption and decryption
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s -e \"plaintext\" OR %s -d \"ciphertext\"\n", argv[0], argv[0]);
        return 1;
    }

    // Define the Polybius table (Hardcoded)
    PolybiusTable_t table = {{
        {'A', 'B', 'C', 'D', 'E'},
        {'F', 'G', 'H', 'I', 'J'},
        {'K', 'L', 'M', 'N', 'O'},
        {'P', 'Q', 'R', 'S', 'T'},
        {'U', 'V', 'W', 'X', 'Y'},
        {'Z', ' ', ' ', ' ', ' '}
    }};

    // Check encryption or decryption mode
    if (strcmp(argv[1], "-e") == 0) {
        pbEncode(argv[2], table);
    } else if (strcmp(argv[1], "-d") == 0) {
        pbDecode(argv[2], table);
    } else {
        fprintf(stderr, "Invalid option: Use -e to encrypt or -d to decrypt.\n");
        return 1;
    }

    return 0;
}
