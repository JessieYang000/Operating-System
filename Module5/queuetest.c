
#include <stdio.h>
#include <queue.h>
#include <process.h>

int main()
{
    queue_t queue = {0, NULL, NULL};

    process_t p1 = {1, "Process One", 100, 1};
    process_t p2 = {1, "Process Two", 200, 2};
    process_t p3 = {1, "Process Three", 300, 3};
    process_t p4 = {1, "Process Four", 400, 4};

    printf("Adding processes into the queue...\n");
    push_queue(&queue, &p1);
    push_queue(&queue, &p2);
    push_queue(&queue, &p3);
    push_queue(&queue, &p4);
    printf("Queue size is: %d.\n", get_queue_size(&queue));

    printf("Poping from the queue. \n");
    process_t *popped_process = pop_queue(&queue);
    printf("Poped proces ID is: %d.\n", popped_process->id);
    printf("Queue size is: %d.\n", get_queue_size(&queue));

    printf("Removing process with highest priority from the queue. \n");
    process_t *highest_process = remove_process(&queue);
    printf("Priority of the highest process is: %d.\n", highest_process->priority);
    printf("Queue size is: %d.\n", get_queue_size(&queue));

    return 0;
}