/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Mutexes
 */
#ifndef KMUTEX_H
#define KMUTEX_H

#include "kproc.h"
#include "queue.h"

// Maximum number of mutexes supported
#define MUTEX_MAX 16

typedef struct mutex_t {
    int allocated;          // Indicates that this mutex has been allocated
    int lock_count;         // The current number of locks held
    proc_t *owner;          // The process that currently holds the mutex
    queue_t wait_queue;     // The processes waiting on the mutex
} mutex_t;

/**
 * Initializes kernel mutex data structures
 * @return -1 on error, 0 on success
 */
int kmutex_init();

/**
 * Allocates a mutex
 * @return -1 on error, otherwise the mutex id that was allocated
 */
int kmutex_alloc();

/**
 * Frees the specified mutex
 * @param id - the mutex id
 * @return 0 on success, -1 on error
 */
int kmutex_free(int id);

/**
 * Locks the specified mutex
 * @param id - the mutex id
 * @return -1 on error, otherwise the current lock count
 */
int kmutex_lock(int id);

/**
 * Unlocks the specified mutex
 * @param id - the mutex id
 * @return -1 on error, otherwise the current lock count
 */
int kmutex_unlock(int id);
#endif
