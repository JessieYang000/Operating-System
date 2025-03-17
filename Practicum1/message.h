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

#endif