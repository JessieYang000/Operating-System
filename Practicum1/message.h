#include "uthash.h" // Include uthash for hash map functionality

// Define struct and functions
#ifndef MESSAGE_H
#define MESSAGE_H

#define CACHE_SIZE = 10;

// Define the Message struct
typedef struct {
    int id;                // Unique identifier
    long timestamp;        // Time when the message was created
    char sender[50];       // Sender's name
    char receiver[50];     // Receiver's name
    char content[256];     // Message content
    int delivered;         // Delivery status (0 = Not delivered, 1 = Delivered)
    UT_hash_handle hh;     // Hash handle for uthash
} Message;

//Hashmap for simulated disk storage
extern Message* disk_store;


// Creates a new message and initializes its fields
Message* create_msg(int id, const char* sender, const char* receiver, const char* content);

//Stores a message persistently in disk
int store_msg(const Message* msg);

//Retrieves a message from disk based on its unique identifier
Message* retrieve_msg(int id);

//Delete a message by ID
void delete_msg(int id);

//Free memory allocations for all messages
void free_all_messages();

#endif