#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "polybius.h"
#include "queue.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>


#define BATCH_SIZE 100
#define MAX_BUFFER_SIZE 10240  // Large enough for a batch of encode

// Function to encode plaintext into Polybius coordinates
void pbEncode(const char *plaintext, PolybiusTable_t table) {
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
void pbDecode(const char *ciphertext, PolybiusTable_t table) {
    printf("Decoded: ");
    for (size_t i = 0; i < strlen(ciphertext); i++) {
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

void process_queue_with_cipher(queue_t *queue, PolybiusTable_t table) {
    int batch_number = 0;
    while (get_queue_size(queue) != 0) {
        int pipefd[2];  // Pipe: parent → child
        int pipefd_out[2];  // Pipe: child → parent

        if (pipe(pipefd) == -1 || pipe(pipefd_out) == -1) {
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // **Child Process**
            close(pipefd[1]);   // Close unused write end of input pipe
            close(pipefd_out[0]); // Close unused read end of output pipe

            // Read input from parent (queue data)
            char input_buffer[MAX_BUFFER_SIZE] = {0};
            read(pipefd[0], input_buffer, sizeof(input_buffer));
            close(pipefd[0]);  // Close input pipe after reading

            // Open pipe for writing output
            FILE *output_pipe = fdopen(pipefd_out[1], "w");
            if (!output_pipe) {
                perror("fdopen failed");
                exit(EXIT_FAILURE);
            }

            // Encrypt and send back
            char *token = strtok(input_buffer, "\n");
            while (token != NULL) {
                // Encode using pbEncode
                for (int i = 0; token[i] != '\0'; i++) {
                    char c = toupper(token[i]);

                    // Preserve spaces
                    if (c == ' ') {
                        fprintf(output_pipe, "  ");
                        continue;
                    }

                    // Look up the character in the Polybius table
                    int found = 0;
                    for (int row = 0; row < ROWS; row++) {
                        for (int col = 0; col < COLS; col++) {
                            if (table.grid[row][col] == c) {
                                fprintf(output_pipe, "%d%d ", row + 1, col + 1);
                                found = 1;
                                break;
                            }
                        }
                        if (found) break;
                    }

                    // If the character is not found, print it unchanged (for symbols)
                    if (!found) {
                        fprintf(output_pipe, "%c ", c);
                    }
                }
                fprintf(output_pipe, "\n");
                token = strtok(NULL, "\n");
            }

            fclose(output_pipe);
            exit(EXIT_SUCCESS);

        } else {  // **Parent Process**
            close(pipefd[0]); // Close unused read end of input pipe
            close(pipefd_out[1]); // Close unused write end of output pipe

            // Collect 100 strings from the queue
            char input_data[MAX_BUFFER_SIZE] = {0};
            int count = 0;
            while (count < BATCH_SIZE && get_queue_size(queue) != 0) {
                char *str = pop_queue(queue);
                strcat(input_data, str);
                strcat(input_data, "\n");
                free(str);
                count++;
            }

            // Send batch data to the child process
            write(pipefd[1], input_data, strlen(input_data));
            close(pipefd[1]);  // Close after sending

            // Read encoded data from child
            char output_data[MAX_BUFFER_SIZE] = {0};
            ssize_t bytes_read = read(pipefd_out[0], output_data, sizeof(output_data) - 1);
            if (bytes_read > 0) {
                output_data[bytes_read] = '\0';

                // Create unique filename
                char filename[50];
                snprintf(filename, sizeof(filename), "output_batch_%d.txt", batch_number++);

                // Save output to file
                int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("File open failed");
                    exit(EXIT_FAILURE);
                }
                write(fd, output_data, bytes_read);
                close(fd);
            }

            close(pipefd_out[0]);  // Close after reading

            // Wait for the child to finish
            wait(NULL);
        }
    }
}

