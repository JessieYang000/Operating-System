// Define struct and functions
#ifndef MESSAGE_H
#define MESSAGE_H

// Define the Message struct
typedef struct {
    int id;                // Unique identifier
    long timestamp;        // Time when the message was created
    char sender[50];       // Sender's name
    char receiver[50];     // Receiver's name
    char content[256];     // Message content
    int delivered;         // Delivery status (0 = Not delivered, 1 = Delivered)
} Message;

// Creates a new message and initializes its fields
Message* create_msg(int id, const char* sender, const char* receiver, const char* content);

//Stores a message persistently in a file
int store_msg(const Message* msg, const char* filename);

//Retrieves a message from a file based on its unique identifier
Message* retrieve_msg(int id, const char* filename);

//Frees the memory allocated for a Message struct
void free_msg(Message* msg); 

#endif