#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


Message* disk_store = NULL; // Hashmap storage initialized as empty

// Create a message
Message *create_msg(int id, const char *sender, const char *receiver, const char *content)
{
    Message *msg = (Message *)malloc(sizeof(Message));

    if (!msg)
    {
        printf("Error: Memory allocation for message failed.\n");
        return NULL;
    }

    msg->id = id;
    msg->timestamp = time(NULL); //Set the current time as the sending time

    //Ensure safe copying by limiting the number of characters copied
    strncpy(msg->sender, sender, sizeof(msg->sender) - 1);
    msg->sender[sizeof(msg->sender) - 1] = '\0'; //Ensure null terminator

    strncpy(msg->receiver, receiver, sizeof(msg->receiver) - 1);
    msg->receiver[sizeof(msg->receiver) - 1] = '\0';

    strncpy(msg->content, content, sizeof(msg->content) - 1);
    msg->content[sizeof(msg->content) - 1] = '\0';

    msg->delivered = 0; // Default set

    return msg;
}


// Store a message in a the simulated disk(hashmap)
int store_msg(const Message* msg) {
    Message* stored_msg;

    HASH_FIND_INT(disk_store, &msg->id, stored_msg);
    if (stored_msg) {
        printf("Message ID %d already exists in storage.\n", msg->id);
        return -1;
    }

    Message* new_msg = (Message*)malloc(sizeof(Message));
    if (!new_msg) {
        printf("Error: Memory allocation failed.\n");
        return -1;
    }

    *new_msg = *msg;  // Copy the message
    HASH_ADD_INT(disk_store, id, new_msg);  // Add to hashmap

    printf("Message ID %d stored in simulated disk.\n", msg->id);
    return 0;
}

// Retrieves a message by ID
Message* retrieve_msg(int id) {
    Message* msg;
    HASH_FIND_INT(disk_store, &id, msg);
    if (!msg) {
        printf("Message ID %d not found.\n", id);
        return NULL;
    }

    // Return a copy to prevent external modification
    Message* result = (Message*)malloc(sizeof(Message));
    if (!result) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    *result = *msg;
    return result;
}

// Deletes a message from hashmap
void delete_msg(int id) {
    Message* msg;
    HASH_FIND_INT(disk_store, &id, msg);
    if (msg) {
        HASH_DEL(disk_store, msg);
        free(msg);
        printf("Message ID %d deleted.\n", id);
    } else {
        printf("Message ID %d not found for deletion.\n", id);
    }
}

// Frees all stored messages
void free_all_messages() {
    Message* current_msg, *tmp;
    HASH_ITER(hh, disk_store, current_msg, tmp) {
        HASH_DEL(disk_store, current_msg);
        free(current_msg);
    }
}