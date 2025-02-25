#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 15

int buffer[BUF_SIZE];
int num = 0;

int total_produced = 0; // Counter for total items produced
int total_consumed = 0; // Counter for total items consumed
int MAX_COUNT = 30;  // The maximum number of items to be produced and consumed
int stop_production = 0; // Flag to stop producers and avoid duplicate messages if two producers reach 30 at the same time
int stop_consumption = 0; // Flag to stop consumers and avoid duplicate messages if two consumers reach 30 at the same time

pthread_mutex_t mut;
pthread_cond_t cond;

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[])
{
    pthread_t t1, t2, t3, t4;
    int id1 = 1, id2 = 2, id3 = 3, id4 = 4; // Unique IDs for thread identification
    if (pthread_mutex_init(&mut, NULL) != 0)
    {
        perror("pthread_mutex_init");
        exit(1);
    }
    if (pthread_cond_init(&cond, NULL) != 0)
    {
        perror("pthread_cond_init");
        exit(1);
    }
    if (pthread_create(&t1, NULL, producer, &id1) != 0)
    {
        perror("pthread_create_t1");
        exit(1);
    }
    if (pthread_create(&t2, NULL, consumer, &id2) != 0)
    {
        perror("pthread_create_t2");
        exit(1);
    }
    if (pthread_create(&t3, NULL, producer, &id3) != 0)
    {
        perror("pthread_create_t3");
        exit(1);
    }
    if (pthread_create(&t4, NULL, consumer, &id4) != 0)
    {
        perror("pthread_create_t4");
        exit(1);
    }
    /* wait a while and then exit */
    sleep(30);
    printf("WE ARE DONE. Total items produced: %d; Total items consumed: %d.\n", total_produced, total_consumed);

    return 0;
}

void *producer(void *param)
{
    int i = 0;
    int id = *(int *)param; // Cast param to int (thread ID)

    while (1)
    {
        pthread_mutex_lock(&mut);
        if (num > BUF_SIZE)
            exit(1);

        // Stop producing if total_produced reaches MAX_COUNT
        if (total_produced >= MAX_COUNT)
        {
            if (!stop_production) {
                printf("Producer %d: Stopping production! 30 items have been produced.\n", id);
                stop_production = 1; // Set stop flag
            }
            pthread_mutex_unlock(&mut);
            break;  // Exit producer thread
        }

        while (num == BUF_SIZE)
            pthread_cond_wait(&cond, &mut);

        buffer[num] = i;
        num++;

        total_produced++;
        printf("Producer %d: Inserted %d at index %d. Total produced: %d\n", id, i, num - 1, total_produced); 

        pthread_mutex_unlock(&mut);
        pthread_cond_signal(&cond);
        sleep(1);

        i++;
    }
    return NULL;
}

void *consumer(void *param)
{
    int i = 0;
    int id = *(int *)param;

    while (1)
    {
        pthread_mutex_lock(&mut);
        if (num < 0)
            exit(1);

        // Stop consuming if total_consumed reaches MAX_COUNT
        if (total_consumed >= MAX_COUNT)
        {
            if (!stop_consumption) {
                printf("Consumer %d: Stopping consumption! 30 items have been consumed.\n", id);
                stop_consumption = 1; 
            }
            pthread_mutex_unlock(&mut);
            break;  // Exit consumer thread
        }
        while (num == 0)
            pthread_cond_wait(&cond, &mut);

        i = buffer[num - 1];                                         
        num--;
        total_consumed++;

        printf("Consumer %d: Consumed %d from index %d. Total consumed: %d\n", id, i, num, total_consumed); 

        pthread_mutex_unlock(&mut);
        pthread_cond_signal(&cond);
        sleep(3);
    }
    return NULL;
}
