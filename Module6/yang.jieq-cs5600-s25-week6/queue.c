
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_queue(queue_t *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
}

// Function to push an element to the end of a queue
void push_queue(queue_t *queue, void *element)
{
    // Check the validness of the queue
    if (!queue)
    {
        perror("Invalid queue.");
        exit(EXIT_FAILURE);
    }

    // Allocate memory to the new node
    queue_node_t *newNode = (queue_node_t *)malloc(sizeof(queue_node_t));

    if (!newNode)
    {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    // Initialize the newNode
    newNode->data = element;
    newNode->prev = queue->tail;
    newNode->next = NULL;

    // Update the queue after adding a new element
    if (queue->size == 0)
    {
        queue->head = newNode;
    }
    else
    {
        queue->tail->next = newNode;
    }

    queue->tail = newNode;

    queue->size++;
}

// Function to remove and return an element fromt the front of a queue
void *pop_queue(queue_t *queue)
{
    if (!queue)
    {
        perror("Invalid queue.");
        exit(EXIT_FAILURE);
    }

    if (queue->size == 0)
    {
        perror("Can't remove from an empty queue.");
        return NULL;
    }

    queue_node_t *removal = queue->head;
    void *data = removal->data;

    queue->head = removal->next;
    if (queue->head)
    {
        queue->head->prev = NULL;
    }
    else
    {
        queue->tail = NULL;
    }

    free(removal); // Free the memory of the removed element
    queue->size--;

    return data;
}

// Function to get the size of a queue
int get_queue_size(queue_t *queue)
{
    if (!queue)
    {
        perror("Invalid queue.");
        exit(EXIT_FAILURE);
    }

    return queue->size;
}

/* A function to save elements from a file to queue*/
int save_in_queue(char *filename, queue_t *queue, int length)
{
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    char *buffer = malloc(length + 2); // Allocate buffer for each line
    if (!buffer)
    {
        perror("Memory allocation failed.");
        fclose(file);
        return 1;
    }

    printf("Saving strings from file %s into queue\n", filename);

    while (fgets(buffer, length + 2, file)) // FIX: Use correct buffer size
    {
        // Remove trailing newline character
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            len--;
        }

        if (len == 0) // Skip empty lines
            continue;

        // Allocate memory for a new line
        char *line = malloc(len + 1);
        if (line == NULL)
        {
            perror("Memory allocation failed");
            free(buffer);
            fclose(file);
            return 1;
        }
        strcpy(line, buffer); // Copy buffer contents

        push_queue(queue, line); // Push the new string into the queue
    }

    printf("%d strings saved in queue\n", get_queue_size(queue));

    free(buffer); // Free the buffer
    fclose(file); // Close the file
    return 0;
}
