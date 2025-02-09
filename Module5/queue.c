
#include <queue.h>
#include <stdlib.h>

// Function to push an element to the end of a queue
void push_queue(queue_t* queue, void* element)
{
    // Check the validness of the queue
    if(!queue) {
        perror("Invalid queue.");
        exit(EXIT_FAILURE);
    }

    // Allocate memory to the new node
    queue_node_t *newNode = (queue_node_t *)malloc(sizeof(queue_node_t)); 

    if(!newNode)
    {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    // Initialize the newNode
    newNode -> data = element;
    newNode -> prev = queue -> tail;
    newNode -> next = NULL;

    // Update the queue after adding a new element
    if(queue -> size == 0) {
        queue -> head = newNode;
    } else {
        queue -> tail -> next = newNode;
    }

    queue -> tail = newNode;

    queue -> size++;
}

// Function to remove and return an element fromt the front of a queue
void* pop_queue(queue_t* queue) {
    if(!queue) {
        perror("Invalid queue.");
        exit(EXIT_FAILURE);
    }

    if(queue -> size == 0) {
        perror("Can't remove from an empty queue.");
        return NULL;
    }

    queue_node_t *removal = queue -> head;
    void *data = removal -> data;

    queue -> head = removal -> next;
    if(queue -> head) {
        queue -> head -> prev = NULL;
    } else {
        queue -> tail = NULL;
    }

    free(removal); // Free the memory of the removed element
    queue -> size--;

    return data;
}