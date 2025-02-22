#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];  // Shared buffer
int count = 0;  // Number of items in the buffer

pthread_mutex_t monitor_lock;  // Monitor lock
pthread_cond_t buffer_not_full;   // Condition: Buffer has space
pthread_cond_t buffer_not_empty;  // Condition: Buffer has items

void* producer(void* arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&monitor_lock);  // Acquire lock (enter monitor)

        while (count == BUFFER_SIZE) {
            // Buffer full, wait for space
            pthread_cond_wait(&buffer_not_full, &monitor_lock);
        }

        // Produce item
        buffer[count++] = i;
        printf("Producer produced: %d\n", i);

        pthread_cond_signal(&buffer_not_empty);  // Notify consumer
        pthread_mutex_unlock(&monitor_lock);  // Release lock (exit monitor)

        sleep(1);  // Simulate work
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&monitor_lock);  // Acquire lock (enter monitor)

        while (count == 0) {
            // Buffer empty, wait for item
            pthread_cond_wait(&buffer_not_empty, &monitor_lock);
        }

        // Consume item
        int item = buffer[--count];
        printf("Consumer consumed: %d\n", item);

        pthread_cond_signal(&buffer_not_full);  // Notify producer
        pthread_mutex_unlock(&monitor_lock);  // Release lock (exit monitor)

        sleep(2);  // Simulate work
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    pthread_mutex_init(&monitor_lock, NULL);
    pthread_cond_init(&buffer_not_full, NULL);
    pthread_cond_init(&buffer_not_empty, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&monitor_lock);
    pthread_cond_destroy(&buffer_not_full);
    pthread_cond_destroy(&buffer_not_empty);

    return 0;
}
