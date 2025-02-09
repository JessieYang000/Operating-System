
#include <queue.h>
#include <stdlib.h>

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

// Function to remove and return the process with the highest priority
process_t *remove_process(queue_t *queue)
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

    // Initialize two variables to track the node with the highest priority and traverse the queue
    queue_node_t *highest_priority_node = queue->head;
    queue_node_t *current_node = queue->head;

    // Find the process with the highest priority
    while (current_node)
    {
        process_t *highest_process = (process_t *)highest_priority_node->data;
        process_t *curr_process = (process_t *)current_node->data;

        if (curr_process->priority > highest_process->priority)
        {
            highest_priority_node = current_node;
        }

        current_node = current_node->next;
    }

    // Update the queue after removing the node
    if (highest_priority_node->prev)
    {
        highest_priority_node->prev->next = highest_priority_node->next;
    }
    else
    {
        queue->head = highest_priority_node->next; // The node to be removed is the head of the queue, so update the head
    }

    if (highest_priority_node->next)
    {
        highest_priority_node->next->prev = highest_priority_node->prev;
    }
    else
    {
        queue->tail = highest_priority_node->prev; // The node to be removed is the tail, so update the tail
    }

    process_t *data = (process_t *)highest_priority_node->data;
    free(highest_priority_node);
    queue->size--;

    return data;
}

// Function to get the size of a queue
int get_queue_size(queue_t* queue) {
    if(!queue) {
        perror("Invalid queue.");
        exit(EXIT_FAILURE);
    }

    return queue->size;
}