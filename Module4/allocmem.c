#include <stdio.h>
#include <stdlib.h>

/* Allocate 5 MB of storage from the static segment: Statically allocated memory exists throughout the program’s lifetime. It is automatically freed by the OS when the program exits.*/
static char static_segment[5 * 1024 * 1024];

// Allocate 1MB from the stack
void allocate_stack_memory()
{
    char stack_mem[1024 * 1024]; // 1 MB
    printf("Stack memory allocated at: %p\n", (void *)stack_mem);
}

int main()
{
    printf("Allocating memories...\n");

    /* Allocate 10 megabyte of storage from the heap: Any memory allocated using malloc must be explicitly freed with free(). If free() is not called, the OS will reclaim the memory automatically when the process terminates. */
    char *heap_mem = (char *)malloc(10 * 1024 * 1024);
    if (heap_mem == NULL)
    {
        perror("Failed to allocate heap memory.");
        return 1;
    }

    printf("Heap memory allocated at: %p\n", (void *)heap_mem);
    printf("Static memory allocated at: %p\n", (void *)static_segment);

    /* Allocate memory from the stack: Stack memory is automatically allocated when a function is called and automatically freed when the function returns. So no need for free()*/
    allocate_stack_memory();

    // Free the heap memory
    free(heap_mem);
    printf("Heap memory freed.\n");

    printf("Program exiting...\n");

    /* Once the program exits, the operating system reclaims all allocated memory (heap, stack, static). The OS removes the process from memory, and any unreleased memory is freed.*/
    return 0;
}

/* What happened when the program exits or the associated process is deleted? 
1. Heap memory(allocated by malloc()): Freed by free(), but even if not freed, the OS reclaims it.
2. Stack memory (local variables): Automatically released when functions return, and fully cleared when the process exits.
3. Static segment: Reclaimed when the process is removed.

Even if the program does not free its memory manually, the OS ensures no memory leak persists after termination.
*/