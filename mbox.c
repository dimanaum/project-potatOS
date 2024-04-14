/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Message exchange via mailboxes
 */

#include <spede/string.h>

#include "mbox.h"
#include "msg.h"
#include "queue.h"

mbox_t mailboxes[MBOX_MAX];

/**
 * Initializes mailboxes
 */
void mbox_init() {
    int i = 0;
    // Initialize mailbox related data structures
    memset(&mailboxes, 0, sizeof(mailboxes));

    // Initialize the mutex queue
    for (i = 0; i < MBOX_MAX; i++)
        queue_init(&(mailboxes[i].wait_queue));
}

/**
 * Queues a message into the given mailbox
 *
 * @param mbox - mailbox number
 * @param msg - pointer to the message data struct to be copied from
 * @return 0 on success, -1 on error
 */
int mbox_queue_in(int mbox, msg_t *msg) {
    // Ensure that mailbox is valid
    // Ensure that the message is valid
    if(msg == NULL) {
        return -1;
    }

    // Return an error if the mailbox is full
    if(mbox >= MBOX_MAX) {
        return -1;
    }

    // Copy the message from the passed in pointer to the tail of the mailbox
    mailboxes[mbox].messages[mailboxes[mbox].tail] = *msg;

    // Move the tail forward
    mailboxes[mbox].tail++;

    // If we are at the end of the array, move the tail to the beginning
    if(mailboxes[mbox].tail >= MBOX_SIZE) {
        mailboxes[mbox].tail = 0;
    }

    // Increment size (since we just added an item to the mailbox)
    mailboxes[mbox].size++;

    return 0;
}

/**
 * De-queues a message out of the given mailbox
 *
 * @param mbox - mailbox number
 * @param msg - pointer to the message data struct to be copied to
 * @return 0 on success, -1 on error
 */
int mbox_queue_out(int mbox, msg_t *msg) {
    // Ensure that mailbox is valid
    // Ensure that the message is valid
    if(msg == NULL) {
        return -1;
    }

    // return an error if mailbox is empty
    if(mbox >= MBOX_MAX) {
        return -1;
    }

    // Copy the message from the head of the mailbox to the passed in message pointer
    *msg = (mailboxes[mbox].messages[mailboxes[mbox].head]);

    // Clear the memory for the message in the mailbox
    memset(&(mailboxes[mbox].messages[mailboxes[mbox].head]), 0, sizeof(msg_t));

    // Move the head forward
    mailboxes[mbox].head++;

    // If we are at the end of the array, move the head to the beginning
    if(mailboxes[mbox].head >= MBOX_SIZE) {
        mailboxes[mbox].head = 0;
    }

    // Decrement size (since we just removed a message from the mailbox)
    mailboxes[mbox].size--;

    return 0;
}
