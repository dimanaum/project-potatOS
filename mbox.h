/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Message exchange via mailboxes
 */
#ifndef MBOX_H
#define MBOX_H

#include "msg.h"
#include "queue.h"

#define MBOX_MAX 10     // Maximum number of mailboxes supported
#define MBOX_SIZE 64    // Maximum number of messages possible in each mailbox

typedef struct {
    msg_t messages[MBOX_SIZE];      // Messages
    int head;                       // First message
    int tail;                       // Last message
    int size;                       // Size of the message queue
    queue_t wait_queue;             // Processes waiting for messages
} mbox_t;

/**
 * Initializes mailboxes
 */
void mbox_init();

/**
 * Queues a message into the given mailbox
 *
 * @param mbox - mailbox number
 * @param msg - pointer to the message from the calling process
 * @return 0 on success, -1 on error
 */
int mbox_queue_in(int mbox, msg_t *msg);

/**
 * De-queues a message out of the given mailbox
 *
 * @param mbox - mailbox number
 * @param msg - pointer to the message from the calling process
 * @return 0 on success, -1 on error
 */
int mbox_queue_out(int mbox, msg_t *msg);

#endif
