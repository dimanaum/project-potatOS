/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * System call APIs
 */
#ifndef SYSCALL_H
#define SYSCALL_H

#include "syscall_defs.h"
#include "msg.h"

/**
 * Executes a new process
 */
int proc_exec(void *ptr, char *name);

/**
 * Exits the current process
 */
void proc_exit(void);

/**
 * Gets the current system time (in seconds)
 * @return system time in seconds
 */
int sys_get_time(void);

/**
 * Gets the current process' id
 * @return process id
 */
int proc_get_pid(void);

/**
 * Gets the current process' name
 * @param name - pointer to a character buffer where the name will be copied
 * @return 0 on success, -1 or other non-zero value on error
 */
int proc_get_name(char *name);

/**
 * Puts the current process to sleep for the specified number of seconds
 * @param seconds - number of seconds the process should sleep
 */
void sleep(int seconds);

/**
 * Allows a process to indicate that it can relinquish control of the CPU
 */
void yield(void);

/**
 * Allocates a mutex from the kernel
 * @return -1 on error, all other values indicate the mutex id
 */
int mutex_alloc(void);

/**
 * Frees a mutex
 * @return -1 on error, 0 on sucecss
 */
int mutex_free(int mutex);

/**
 * Locks the mutex
 * @param *mutex - pointer to the mutex identifer
 * @return -1 on error, 0 on sucecss
 * @note If the mutex is already locked, process will block/wait.
 */
int mutex_lock(int mutex);

/**
 * Unlocks the mutex
 * @return -1 on error, 0 on sucecss
 * @param *mutex - pointer to the mutex identifier
 */
int mutex_unlock(int mutex);

#endif

/**
 * Sends a message to the specified mailbox
 * @param mbox - Mailbox number to send to
 * @param msg - Pointer to the message data structure
 * @return -1 on error, 0 on success
 */
int msg_send(int mbox, msg_t *msg);

/**
 * Receives a message from the specified mailbox
 * This call is blocking; if no message exists in the mailbox, the system
 * call will block (process moves to WAITING state)
 *
 * @param mbox - Mailbox number to send to
 * @param msg - Pointer to the message data structure
 * @return -1 on error, 0 on success
 */
int msg_recv(int mbox, msg_t *msg);

