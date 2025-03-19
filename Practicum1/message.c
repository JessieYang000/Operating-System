#include "message.h"
#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h> // Check if a directory exists or create a directory
#include <unistd.h>

// Cache variables
Message* cache = NULL;  // Hashmap for fast lookup
Message* head = NULL;  // Head of LRU list
Message* tail = NULL;  // Tail of LRU list
int cache_count = 0;   // Number of messages in cache

// Cache lookup function
Message* cache_lookup(int id) {
    Message* msg;
    HASH_FIND_INT(cache, &id, msg);
    if (msg) {
        // Move accessed message to the front of the LRU list
        if (msg != head) {
            // Remove from current position
            if (msg->prev) msg->prev->next = msg->next;
            if (msg->next) msg->next->prev = msg->prev;
            if (msg == tail) tail = msg->prev;

            // Move to front
            msg->next = head;
            msg->prev = NULL;
            if (head) head->prev = msg;
            head = msg;
        }
        return msg;
    }
    return NULL;
}

//Insert a message into the cache
void cache_insert(Message* msg) {
    // Check if message is already in cache
    if (cache_lookup(msg->id)) return;

    // If cache is full, evict LRU message
    if (cache_count >= CACHE_SIZE) {
        cache_remove(tail->id);
    }

    // Insert message into hashmap
    Message* new_msg = (Message*)malloc(sizeof(Message));
    *new_msg = *msg;
    HASH_ADD_INT(cache, id, new_msg);

    // Insert message at the front of LRU list
    new_msg->next = head;
    new_msg->prev = NULL;
    if (head) head->prev = new_msg;
    head = new_msg;
    if (!tail) tail = new_msg; // If cache was empty, set tail
    cache_count++;
}

// Remove the least visited message from the cache
void cache_remove(int id) {
    Message* msg;
    HASH_FIND_INT(cache, &id, msg);
    if (!msg) return;

    // Remove from LRU list
    if (msg->prev) msg->prev->next = msg->next;
    if (msg->next) msg->next->prev = msg->prev;
    if (msg == head) head = msg->next;
    if (msg == tail) tail = msg->prev;

    // Remove from hashmap
    HASH_DEL(cache, msg);
    free(msg);
    cache_count--;
}

// Remove all message from the cache(both from the LRU linked list and the hashmap) and free allocated memory
void cache_free() {
    Message* current_msg, *tmp;

    HASH_ITER(hh, cache, current_msg, tmp) {
        HASH_DEL(cache, current_msg);  // Remove from hash table
        free(current_msg);  // Free memory
    }

    // Reset cache state
    cache = NULL;
    head = NULL;
    tail = NULL;
    cache_count = 0;
    
    printf("Cache cleared.\n");
}

// Helper function to generate the filename for a message, the last parameter prevents buffer overflows
void get_message_filename(int id, char *filename, size_t size)
{   
    // .dat files store binary representations of Message structs for faster write and read
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
    struct stat st = {0}; // Declare a struct to hold file status
    if (stat(MESSAGE_DIR, &st) == -1) // Check if directory exists
    {
        // Create directory with permissions 0755 (allow to read and execute)
        mkdir(MESSAGE_DIR, 0755); 
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

    // Write the entire message struct to the file in binary format which is faster than fprintf()
    fwrite(msg, sizeof(Message), 1, file);
    fclose(file);

    printf("Message ID %d stored in file %s.\n", msg->id, filename);
    return 0;
}

// Retrieves a message by ID
Message *retrieve_msg(int id)
{
    // Check cache first
    Message* msg = cache_lookup(id);
    if (msg) {
        printf("Cache hit: Message ID %d found in cache.\n", id);
        return msg;
    }

    // If not found in cache, search from the disk
    char filename[100];
    get_message_filename(id, filename, sizeof(filename));

    // Open file for reading(binary mode)
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Message ID %d not found on disk.\n", id);
        return NULL;
    }

    // Allocate memory
    msg = (Message*)malloc(sizeof(Message));

    // Read the entire message struct from the file
    fread(msg, sizeof(Message), 1, file);
    fclose(file);

    //Insert into cache
    cache_insert(msg);
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
    }
    else
    {
        printf("Error: Message ID %d not found for deletion.\n", id);
    }
}

// Free dynamically allocated messages
void free_msg(Message* msg) {
    if (msg) {
        free(msg);
    }
}