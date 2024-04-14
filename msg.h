/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Message Data Structures
 */
#ifndef MSG_H
#define MSG_H

#define MSG_SIZE 256

typedef struct msg_t {
    int sender;                     // Process ID that sent the message
    int time_sent;                  // Time that message was sent
    int time_received;              // Time that message was received
    unsigned char data[MSG_SIZE];   // Message data
} msg_t;

#endif
