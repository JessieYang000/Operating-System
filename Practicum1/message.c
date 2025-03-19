#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Helper function to generate the filename for a message
void get_message_filename(int id, char *filename, size_t size)
{
    snprintf(filename, size, "%smessage_%d.dat", MESSAGE_DIR, id);
}

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
    msg->timestamp = time(NULL); // Set the current time as the sending time

    // Ensure safe copying by limiting the number of characters copied
    strncpy(msg->sender, sender, sizeof(msg->sender) - 1);
    msg->sender[sizeof(msg->sender) - 1] = '\0'; // Ensure null terminator

    strncpy(msg->receiver, receiver, sizeof(msg->receiver) - 1);
    msg->receiver[sizeof(msg->receiver) - 1] = '\0';

    strncpy(msg->content, content, sizeof(msg->content) - 1);
    msg->content[sizeof(msg->content) - 1] = '\0';

    msg->delivered = 0; // Default set

    return msg;
}

// Store a message in a file-per-message disk
int store_msg(const Message *msg)
{
    // Ensure the messages directory exists
    struct stat st = {0};
    if (stat(MESSAGE_DIR, &st) == -1)
    {
        mkdir(MESSAGE_DIR, 0700);
    }

    // Generate the filename
    char filename[100];
    get_message_filename(msg->id, filename, sizeof(filename));

    // Open file for writing
    FILE *file = fopen(filename, "wb"); // Write only(binary, overwrites existing file)
    if (!file)
    {
        printf("Error: Could not open file %s for writing.\n", filename);
        return -1;
    }

    // Write the entire message struct to the file
    fwrite(msg, sizeof(Message), 1, file);
    fclose(file);

    printf("Message ID %d stored in file %s.\n", msg->id, filename);
    return 0;
}

// Retrieves a message by ID
Message *retrieve_msg(int id)
{
    // Generate the filename
    char filename[100];
    get_message_filename(id, filename, sizeof(filename));

    // Open file for reading
    FILE *file = fopen(filename, "rb"); // Read only(binary)
    if (!file)
    {
        printf("Message ID %d not found on disk.\n", id);
        return NULL;
    }

    // Allocate memory for the message
    Message *msg = (Message *)malloc(sizeof(Message));
    if (!msg)
    {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // Read the message from the file
    fread(msg, sizeof(Message), 1, file);
    fclose(file);

    return msg;
}

// Deletes a message from hashmap
void delete_msg(int id)
{
    // Generate the filename
    char filename[100];
    get_message_filename(id, filename, sizeof(filename));

    // Remove the file
    if (remove(filename) == 0)
    {
        printf("Message ID %d deleted.\n", id);
        return 0;
    }
    else
    {
        printf("Error: Message ID %d not found for deletion.\n", id);
        return -1;
    }
}