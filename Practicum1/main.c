#include "message.h"
#include <stdio.h>
#include <time.h>

void print_msg(Message *msg)
{
    if (!msg)
    {
        printf("Message not found.\n");
        return;
    }
    printf("ID: %d | Timestamp: %ld | Sender: %s | Receiver: %s | Content: %s | Delivered: %d\n",
           msg->id, msg->timestamp, msg->sender, msg->receiver, msg->content, msg->delivered);
}

int main()
{
    srand(time(NULL)); // For randomly generating 1000 message access

    printf("\n--- TEST 1: Store and Retrieve Messages (Disk + Cache) ---\n");
    Message *msg1 = create_msg(1, "Alice", "Bob", "Hello, Bob!");
    Message *msg2 = create_msg(2, "Charlie", "David", "How are you?");
    Message *msg3 = create_msg(3, "Eve", "Mallory", "Secret Message!");

    store_msg(msg1);
    store_msg(msg2);
    store_msg(msg3);

    int isLRU = 1;
    cache_insert(msg1, isLRU);
    cache_insert(msg2, isLRU);

    printf("Retrieving from cache:\n");
    Message *retrieved_msg1 = retrieve_msg(1, isLRU);
    print_msg(retrieved_msg1); // Should be a cache hit

    Message *retrieved_msg2 = retrieve_msg(2, isLRU);
    print_msg(retrieved_msg2); // Should be a cache hit
    printf("\n----------------------------------------\n");

    printf("\n--- TEST 2: Cache Miss and Disk Read ---\n");
    Message *retrieved_msg3 = retrieve_msg(3, isLRU);
    print_msg(retrieved_msg3); // Should be a cache hit and read from disk

    Message *retrieved_msg4 = retrieve_msg(4, isLRU);
    print_msg(retrieved_msg4); // Should be a cache miss (not in disk)
    printf("\n----------------------------------------\n");

    printf("\n--- TEST 3: Cache Eviction (Exceeding Cache Size) ---\n");
    // Filling the cache beyond CACHE_SIZE
    for (int i = 4; i <= 20; i++)
    {
        Message *new_msg = create_msg(i, "Sender", "Receiver", "This is a test message.");
        store_msg(new_msg);
        cache_insert(new_msg, isLRU);
        free_msg(new_msg);
    }

    // Retrieving an old message should cause a cache miss
    printf("Checking eviction by accessing an old message (ID 1)...\n");
    Message *old_msg = retrieve_msg(1, isLRU); // ID 1 should have been evicted
    print_msg(old_msg);

    if (old_msg)
        free(old_msg);
    printf("\n----------------------------------------\n");

    printf("\n--- TEST 4: Deleting Messages ---\n");
    Message *msg = create_msg(100, "Charlie", "David", "How are you?");
    store_msg(msg);
    cache_insert(msg, isLRU);

    // Remove from cache
    printf("Cache count before deletion: %d\n", get_cache_count());
    cache_remove(100);
    printf("Cache count after deletion: %d\n", get_cache_count()); // Should decrement by 1

    Message *retrieved_after_deletion = retrieve_msg(100, isLRU);
    print_msg(retrieved_after_deletion); // Should be a cache miss but found in disk
    printf("\n----------------------------------------\n");

    printf("\n--- TEST 5: Flushing the Cache ---\n");
    printf("Cache count before flushing: %d\n", get_cache_count()); // Should be 16
    cache_free();
    printf("Cache count before flushing: %d\n", get_cache_count()); // Should be 0

    // Free all dynamically allocated messages
    free_msg(msg1);
    free_msg(msg2);
    free_msg(msg3);
    free_msg(retrieved_msg4);

    printf("\n--- TEST 6: Cache Eviction by Random Replacement ---\n");
    isLRU = 0;
    for (int i = 1; i <= 20; i++)
    {
        Message *new_msg = create_msg(i, "Sender", "Receiver", "This is a test message.");
        store_msg(new_msg);
        cache_insert(new_msg, isLRU);
        free_msg(new_msg);
    }

    printf("\n--- TEST 7: Cache Hit and Cache Miss ---\n");
    for (int i = 1; i <= 200; i++)
    {
        Message *new_msg = create_msg(i, "Sender", "Receiver", "This is a test message.");
        cache_insert(new_msg, isLRU);
        free_msg(new_msg);
    }

    int hitCount = 0;
    int missCount = 0;

    for (int i = 0; i < 1000; i++)
    {
        int rand_id = (rand() % 200) + 1;
        Message *msg = cache_lookup(rand_id); // Only tests hit/miss
        if (msg)
        {
            hitCount++;
        }
        else
        {
            missCount++;
        }
    }
    printf("Number of hits: %d\n", hitCount);
    printf("Number of misses: %d\n", missCount);
    printf("Hit Ratio: %.2f%%\n", (hitCount / 1000.0) * 100);

    cache_free();
    printf("\n--- All Tests Completed Successfully ---\n");
    return 0;
}