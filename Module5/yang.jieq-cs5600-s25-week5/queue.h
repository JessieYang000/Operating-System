#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include "process.h"

// Forward declaration of queue_node_t
typedef struct queue_node queue_node_t;

// Definition of queue_node_t
struct queue_node
{
    void *data;
    queue_node_t *prev;
    queue_node_t *next;
};

typedef struct
{
    int size;
    queue_node_t *head;
    queue_node_t *tail;
} queue_t;

void push_queue(queue_t *queue, void *element);
void *pop_queue(queue_t *queue);
process_t *remove_process(queue_t *queue);
int get_queue_size(queue_t *queue);

#endif