// Define struct and functions
#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_DIR "messages/" // Directory to store message files
#define CACHE_SIZE 16  // Number of messages in cache
#define MESSAGE_SIZE 1024  // Fixed message size in bytes

#include "uthash.h"

// Define the Message struct
typedef struct {
    int id;                // Unique identifier
    long timestamp;        // Time when the message was created
    char sender[50];       // Sender's name
    char receiver[50];     // Receiver's name
    char content[256];     // Message content
    int delivered;         // Delivery status (0 = Not delivered, 1 = Delivered)
    struct Message* prev;  // For doubly linked list (LRU)
    struct Message* next;  // For doubly linked list (LRU)
    UT_hash_handle hh;     // UTHash handle for fast lookup
} Message;

/**
 * Function declarations related to Message structure
 */
// Creates a new message and initializes its fields
Message* create_msg(int id, const char* sender, const char* receiver, const char* content);

//Stores a message persistently in disk
int store_msg(const Message* msg);

//Retrieves a message from disk based on its unique identifier
Message* retrieve_msg(int id);

//Delete a message by ID
void delete_msg(int id);

// Free the memory allocation for a message
void free_msg(Message* msg);

/**
 * Function declarations related to cache operations
 */

void init_cache();
Message* cache_lookup(int id);
void cache_insert(Message* msg);
void cache_remove(int id);
void cache_free();
#endif