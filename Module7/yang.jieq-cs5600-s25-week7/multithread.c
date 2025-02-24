#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ARRAY_SIZE 5

int shared_array[ARRAY_SIZE]; // Shared memory (global array)

void *worker(void *data)
{
    char *name = (char *)data;

    if (*name == 'X')
    { // Writer thread (X)
        printf("Thread %s started writing\n", name);
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            shared_array[i] = i * 10 + (rand() % 10); // Assign different values
            printf("Writer: Wrote %d at index %d\n", shared_array[i], i);
            usleep(50000);
        }
        printf("Writer Thread %s done!\n", name);
    }
    else if (*name == 'Y')
    { // Reader thread (Y)
        printf("Thread %s started reading\n", name);
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("Reader: Read %d from index %d\n", shared_array[i], i);
            usleep(70000);
        }
        printf("Reader Thread %s done!\n", name);
    }
    return NULL;
}

// int main(void)
// {
//     pthread_t th1, th2;
//     // Both pthread_create() calls return immediately, and both threads start running concurrently (simultaneously).
//     /*pthread_create(&th1, NULL, worker, "X") and pthread_create(&th2, NULL, worker, "Y") create two threads (th1 and th2).
// These threads start executing worker immediately.
// The main thread then sleeps for 5 seconds (sleep(5)), allowing worker to run during this time.
// After 5 seconds, the main thread exits, causing the entire program to terminate.
// If worker was still running, it gets forcefully stopped when the main thread exits. */
//     pthread_create(&th1, NULL, worker, "X");
//     pthread_create(&th2, NULL, worker, "Y");
//     sleep(5);
//     printf("Exiting from main program\n");

//     return 0;
// }

// /*Before: Without pthread_join(), the program exits after 5 seconds, killing both threads (th1 and th2) prematurely.
// After: Now, even though sleep(5) executes first, pthread_join() ensures that the main thread waits for both threads to complete before exiting. */
// int main(void)
// {
//     pthread_t th1, th2;
//     pthread_create(&th1, NULL, worker, "X");
//     pthread_create(&th2, NULL, worker, "Y");
//     sleep(5);
//     printf("exiting from main program\n");
//     pthread_join(th1, NULL); // Wait for thread X to finish
//     pthread_join(th2, NULL); // Wait for thread Y to finish
//     return 0;
// }

int main(void)
{
    pthread_t th1, th2;
    pthread_create(&th1, NULL, worker, "X");
    pthread_create(&th2, NULL, worker, "Y");
    sleep(1);
    // Print thread ID before cancelling
    printf("====> Cancelling Thread Y (ID: %lu)!!\n", (unsigned long)th2);
    pthread_cancel(th2);
    usleep(100000);
    // Print thread ID before cancelling
    printf("====> Cancelling Thread X (ID: %lu)!\n", (unsigned long)th1);
    pthread_cancel(th1);
    printf("exiting from main program\n");
    return 0;
}