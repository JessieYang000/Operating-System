#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


Message* disk_storage = NULL; // Hashmap storage initialized as empty

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
    strcpy(msg->sender, sender, sizeof(mes->sender) - 1);
    msg->sender[sizeof(msg->sender) - 1] = '\0'; //Ensure null terminator

    strcpy(msg->receiver, receiver, sizeof(mes->receiver) - 1);
    msg->receiver[sizeof(msg->receiver) - 1] = '\0';

    strcpy(msg->content, content, sizeof(mes->content) - 1);
    msg->content[sizeof(msg->content) - 1] = '\0';

    msg->delivered = 0; // Default set

    return msg;
}


// Store a message in a the simulated disk(hashmap)
int store_msg(const Message* msg) {
    Message* stored_msg;

    HASH_FIND_INT(disk_storage, &msg->id, stored_msg);
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
    HASH_ADD_INT(disk_storage, id, new_msg);  // Add to hashmap

    printf("Message ID %d stored in simulated disk.\n", msg->id);
    return 0;
}