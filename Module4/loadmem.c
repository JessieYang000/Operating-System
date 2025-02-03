
#include <stdlib.h>
#include <stdio.h>

typedef struct  {
    int *data; // Pointer to the dynamically allocated array
    int size; // Size of the allocated array
} dyn_block;

dyn_block* alloc_dyn_block(int size) {
    dyn_block *block = (dyn_block *)malloc(sizeof(dyn_block));
    if(!block) {
        fprintf(stderr, "Memory allocation failed for dyn_block\n");
        return NULL;
    }

    block->data = (int *)malloc(size * sizeof(int));
    if(!block->data) {
        fprintf(stderr, "Memory allocation failed for data array\n");
        free(block);
        return NULL;
    }

    block->size = size;
    return block;
}

void store_mem_blk(dyn_block *block, int *values, int size) {
    if(!block || !block->data) {
        fprintf(stderr, "Invalid dyn_block provided\n");
        return;
    }

    if(size > block->size) {
        fprintf(stderr, "Input size exceeds size of the allocated memory block\n");
        return;
    }

    for(int i = 0; i < size; i++) {
        block->data[i] = values[i];
    }
}

int main() {
    int size = 10;
    dyn_block *block = alloc_dyn_block(size);

    if(!block) {
        puts("Failed to allocate memory of a block");
        return EXIT_FAILURE;
    }

    int sample_data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    store_mem_blk(block, sample_data, size);

    puts("Stored data into dyn_block\n");
    for(int i = 0; i < block->size; i++) {
        printf("%d", block->data[i]);
    }
    puts("\n");

    free(block->data);
    free(block);
            
    return EXIT_SUCCESS;
}