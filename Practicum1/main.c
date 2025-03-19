#include "message.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Create and store messages
    Message* msg1 = create_msg(1, "Alice", "Bob", "Hello, Bob!");
    store_msg(msg1);

    Message* msg2 = create_msg(2, "Charlie", "David", "How are you?");
    store_msg(msg2);

    // Retrieve a message
    Message* retrieved = retrieve_msg(1);
    if (retrieved) {
        printf("Retrieved Message ID %d: %s -> %s, Content: %s\n",
               retrieved->id, retrieved->sender, retrieved->receiver, retrieved->content);
        free(retrieved);
    }

    // Delete a message
    delete_msg(1);

    // Try retrieving a deleted message
    retrieved = retrieve_msg(1);
    if (!retrieved) {
        printf("Message ID 1 not found as expected.\n");
    }

    // Clean up dynamically allocated messages
    free(msg1);
    free(msg2);

    return 0;
}
