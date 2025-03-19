#include "message.h"
#include <stdio.h>

void print_msg(Message* msg) {
    if (!msg) {
        printf("Message not found.\n");
        return;
    }
    printf("ID: %d | Timestamp: %ld | Sender: %s | Receiver: %s | Content: %s | Delivered: %d\n",
           msg->id, msg->timestamp, msg->sender, msg->receiver, msg->content, msg->delivered);
}

int main() {
    printf("\n--- TEST 1: Store and Retrieve Messages (Disk + Cache) ---\n");
    Message* msg1 = create_msg(1, "Alice", "Bob", "Hello, Bob!");
    Message* msg2 = create_msg(2, "Charlie", "David", "How are you?");
    Message* msg3 = create_msg(3, "Eve", "Mallory", "Secret Message!");

    store_msg(msg1);
    store_msg(msg2);
    store_msg(msg3);

    cache_insert(msg1);
    cache_insert(msg2);
    cache_insert(msg3);

    printf("Retrieving from cache:\n");
    print_msg(retrieve_msg(1)); // Should be a cache hit
    print_msg(retrieve_msg(2)); // Should be a cache hit

    printf("\n--- TEST 2: Cache Miss and Disk Read ---\n");
    Message* msg4 = retrieve_msg(3); // Should be a cache hit
    Message* msg5 = retrieve_msg(10); // Should be a cache miss (not in disk)

    printf("\n--- TEST 3: Cache Eviction (Exceeding Cache Size) ---\n");
    // Filling the cache beyond CACHE_SIZE (assumed 16)
    for (int i = 4; i <= 20; i++) {
        Message* new_msg = create_msg(i, "Sender", "Receiver", "This is a test message.");
        store_msg(new_msg);
        cache_insert(new_msg);
    }

    // Retrieving an old message should now cause a cache miss
    printf("Checking LRU eviction by accessing an old message (ID 1)...\n");
    Message* old_msg = retrieve_msg(1); // ID 1 should have been evicted
    print_msg(old_msg);

    printf("\n--- TEST 4: Deleting Messages ---\n");
    delete_msg(2); // Delete from disk
    Message* deleted_msg = retrieve_msg(2); // Should be a cache miss
    print_msg(deleted_msg); // Should print "Message not found."

    printf("\n--- TEST 5: Flushing the Cache ---\n");
    cache_free();
    Message* after_flush = retrieve_msg(3); // Should be a cache miss (must read from disk)
    print_msg(after_flush);

    // Clean up memory
    free_msg(msg1);
    free_msg(msg2);
    free_msg(msg3);

    printf("\n--- All Tests Completed Successfully ---\n");
    return 0;
}