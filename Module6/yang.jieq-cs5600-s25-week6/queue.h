#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>

/*Forward declaration of queue_node_t: The forward declaration typedef struct queue_node queue_node_t; informs the compiler about the existence of a struct queue_node type without providing its complete definition at that point. This is particularly useful when you have self-referential structures or when two structures reference each other, as it allows the compiler to recognize the type before its full definition is encountered.
*/

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

void init_queue(queue_t *queue);
void push_queue(queue_t *queue, void *element);
void *pop_queue(queue_t *queue);
int get_queue_size(queue_t *queue);
int save_in_queue(char *filename, queue_t *queue, int length);

#endif