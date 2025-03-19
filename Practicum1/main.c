#include "message.h"
#include <stdio.h>

int main() {
    // Create and store messages
    Message* msg1 = create_msg(1, "Alice", "Bob", "Hello, Bob!");
    store_msg(msg1);
    cache_insert(msg1);

    Message* msg2 = create_msg(2, "Charlie", "David", "How are you?");
    store_msg(msg2);
    cache_insert(msg2);


   // Retrieve messages (cache hit/miss testing)
    Message* retrieved = retrieve_msg(1);
    if (retrieved) printf("Message retrieved: %s\n", retrieved->content);

    retrieved = retrieve_msg(2);
    if (retrieved) printf("Message retrieved: %s\n", retrieved->content);

    retrieved = retrieve_msg(3); // Should cause a cache miss
    if (!retrieved) printf("Message ID 3 not found.\n");

    return 0;
}
