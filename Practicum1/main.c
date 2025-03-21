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

    printf("Retrieving from cache:\n");
    Message* retrieved_msg1 = retrieve_msg(1);
    print_msg(retrieved_msg1); // Should be a cache hit

    Message* retrieved_msg2 = retrieve_msg(2);
    print_msg(retrieved_msg2); // Should be a cache hit
    printf("\n----------------------------------------\n");

    printf("\n--- TEST 2: Cache Miss and Disk Read ---\n");
    Message* retrieved_msg3 = retrieve_msg(3);
    print_msg(retrieved_msg3); // Should be a cache hit and read from disk
        
    Message* retrieved_msg4 = retrieve_msg(4);
    print_msg(retrieved_msg4); // Should be a cache miss (not in disk)
    printf("\n----------------------------------------\n");

    printf("\n--- TEST 3: Cache Eviction (Exceeding Cache Size) ---\n");
    // Filling the cache beyond CACHE_SIZE
    for (int i = 4; i <= 20; i++) {
        Message* new_msg = create_msg(i, "Sender", "Receiver", "This is a test message.");
        store_msg(new_msg);
        cache_insert(new_msg);
        free_msg(new_msg);  
    }

    // Retrieving an old message should cause a cache miss
    printf("Checking eviction by accessing an old message (ID 1)...\n");
    Message* old_msg = retrieve_msg(1); // ID 1 should have been evicted
    print_msg(old_msg);

    if(old_msg) 
        free(old_msg);
    printf("\n----------------------------------------\n");

    // printf("\n--- TEST 4: Deleting Messages ---\n");
    // delete_msg(2); // Delete an previously evicted message from disk
    // Message* retrieved_after_deletion = retrieve_msg(2);
    // print_msg(retrieved_after_deletion); // Should print "Message not found."

    // // Delete from both cache and disk
    // cache_remove(12);
    // delete_msg(12); 
    // Message* another_retrieved_after_deletion = retrieve_msg(12);

    // print_msg(another_retrieved_after_deletion); // Should be a cache miss and print "Message not found."
    // printf("\n----------------------------------------\n");

    // printf("\n--- TEST 5: Flushing the Cache ---\n");
    // cache_free();
    // // Should be a cache miss (must read from disk)
    // print_msg(retrieve_msg(20));

    // // Free all dynamically allocated messages
    // free_msg(msg1);
    // free_msg(msg2);
    // free_msg(msg3);

    // free_msg(retrieved_msg1);
    // free_msg(retrieved_msg2);
    // free_msg(retrieved_msg3);
    // free_msg(retrieved_msg4);
    // free_msg(old_msg);
    // free_msg(retrieved_after_deletion);
    // free_msg(another_retrieved_after_deletion);

    // printf("\n--- All Tests Completed Successfully ---\n");
    // return 0;
}