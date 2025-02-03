
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024  // Safe max length for fgets; for long lines, getline() will work by dynamically allocate the memory

// Define a struct to store an array of integers dynamically
typedef struct
{
    int *data; // Pointer to the dynamically allocated array
    int size;  // Size of the allocated array
} dyn_block;

/*
A function to define a dyn_block object with given predefined size
*/
dyn_block *alloc_dyn_block(int size)
{
    // Allocate memory from the heap for the block pointer
    dyn_block *block = (dyn_block *)malloc(sizeof(dyn_block));

    // Validate the block after memory allocation
    if (!block) // Same as block == NULL
    {
        fprintf(stderr, "Memory allocation failed for dyn_block\n");
        return NULL;
    }

    // Allocate memory from the heap for the data variable of the block object
    block->data = (int *)malloc(size * sizeof(int));

    // Validate the memory allocatio for the data pointer
    if (!block->data)
    {
        fprintf(stderr, "Memory allocation failed for data array\n");

        // Free the block memory allocation if allocation for data failed
        free(block); 
        return NULL;
    }

    // Initialize the size variable with the given size
    block->size = size;
    return block;
}

/*
A function to store values from an integer array into the dynamic block
*/
void store_mem_blk(dyn_block *block, int *values, int size)
{
    // Ensure the dynamic block and its data array are valid
    if (!block || !block->data)
    {
        fprintf(stderr, "Invalid dyn_block provided\n");
        return;
    }
    
    // Validate the input integer array pointer
    if (!values)
    {
        fprintf(stderr, "Invalid values pointer provided\n");
        return;
    }

    // Check if size is negative
    if (size < 0) {
        fprintf(stderr, "Invalid size: cannot be negative\n");
        return;
    }

    // Prevent writing beyond allocated memory
    if (size > block->size)
    {
        fprintf(stderr, "Input size exceeds size of the allocated memory block\n");
        return;
    }

    // Copy values into the allocated memory block
    for (int i = 0; i < size; i++)
    {
        block->data[i] = values[i];
    }
}

/*
A function to convert a line of numbers into an integer array
*/

int convert_line_to_int_array(char *line, int **values) {
    int count = 0;
    char *token;

    char *temp = strdup(line); // Avoid modifying the original pointer
    if(!temp) return 0;

    //Count the numbers in a line
    token = strtok(temp, " ");
    while(token) {
        count++;
        token = strtok(NULL, " ");
    }

    free(temp); // Free temporary memory if it is unnecessary anymore

    if(count == 0) return 0;

    *values = (int *)malloc(count * sizeof(int));
    if(!*values) {
        fprintf(stderr, "Memory allocation failed for values array\n");
        return 0;
    }

    //Convert string tokens to integers
    token = strtok(line, " ");
    for(int i = 0; i < count; i++) {
        if(token) {
            (*values)[i] = atoi(token);
            token = strtok(NULL, " ");
        }
    }

    return count;
}

int main()
{
    // Open the file
    FILE *file = fopen("blocks.data", "r");
    if(!file) {
        perror("Error opening file.");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH]; // Buffer for reading each line
    dyn_block **blocks = NULL; // Array of dyn_block pointers to store all dyn_block objects 
    int block_count = 0;

    // Read the file line by line
    while(fgets(line, sizeof(line), file)) {
        int *values = NULL;
        int num_values = convert_line_to_int_array(line, &values);

        if(num_values > 0) {
            // If found a valid line, then allocate a new dyn_block
            dyn_block *block = alloc_dyn_block(num_values);

            // If allocation failed, free the values pointer and read the next line
            if(!block) {
                if(values) free(values);
                continue;
            }

            // Store the values into the block
            store_mem_blk(block, values, num_values);

            // Add the current block to the list
            dyn_block **temp = realloc(blocks, (block_count + 1) * sizeof(dyn_block *));

            // If blocks allocation failed, free all pointers and exit
            if(!temp) {
                fprintf(stderr, "Memory allocation failed for blocks list.\n");
                free(block -> data);
                free(block);
                if(values) free(values);

                return EXIT_FAILURE;
            }

            blocks = temp;
            blocks[block_count++] = block;
        }

        if(values) free(values); // Free temporaty integer array
    }

    fclose(file);

    // Print stored data to verify correctness
    for(int i = 0; i < block_count; i++) {
        printf("Block %d:", i + 1);
        for(int j = 0; j < blocks[i]->size; j++) {
            printf("%d ", blocks[i]->data[j]);
        }
        puts("\n");
    }

    // Free all memory allocation
    for(int i = 0; i < block_count; i++) {
        free(blocks[i]->data);
        free(blocks[i]);
    }

    if(blocks) free(blocks);
    return EXIT_SUCCESS;
}