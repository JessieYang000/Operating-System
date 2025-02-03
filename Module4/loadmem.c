
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    int *token;

    int *temp = strdup(line); // Avoid modifying the original pointer
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
        (*values)[i] = atoi(token);
        token = strtok(NULL, " ");
    }

    return count;
}

int main()
{
    int size = 10;

    // Define a dyn_block object with predefined size
    dyn_block *block = alloc_dyn_block(size); 

    // Check whether the block was defined successfully or not
    if (!block)
    {
        puts("Failed to allocate memory of a block.");
        return EXIT_FAILURE; // Equivalent to 1
    }

    // Define a sample data array holding integers
    int sample_data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Store the values of the data array into the dynamic block
    store_mem_blk(block, sample_data, size);

    // Print out the values in the block to see whether storing them successfuly
    puts("Stored data into dyn_block.");
    for (int i = 0; i < block->size; i++)
    {
        printf("%d ", block->data[i]);
    }
    
    puts("\n");

    // Free the memory allocated on the heap
    free(block->data);
    free(block);

    return EXIT_SUCCESS;
}