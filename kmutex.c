/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Mutexes
 */

#include <spede/string.h>

#include "kernel.h"
#include "kmutex.h"
#include "queue.h"
#include "kutil.h"
#include "scheduler.h"

// Table of all mutexes
mutex_t mutexes[MUTEX_MAX];

// Mutex ids to be allocated
queue_t mutex_queue;

/**
 * Initializes kernel mutex data structures
 * @return -1 on error, 0 on success
 */
int kmutex_init() {
    int i;

    // Initialize the mutex table
    memset(&mutexes, 0, sizeof(mutexes));

    // Initialize the mutex queue
    queue_init(&mutex_queue);

    // Fill the mutex queue
    for (i = 0; i < MUTEX_MAX; i++) {
        if (queue_in(&mutex_queue, i) != 0) {
            return -1;
        }
    }

    return 0;
}

/**
 * Allocates a mutex
 * @return -1 on error, otherwise the mutex id that was allocated
 */
int kmutex_alloc() {
    int id;
    mutex_t *mutex;

    // Obtain a mutex id
    if (queue_out(&mutex_queue, &id) != 0) {
        return -1;
    }

    // Ensure that the id is within the valid range
    if (id < 0 || id >= MUTEX_MAX) {
        return -1;
    }

    // Pointer to the mutex table entry
    mutex = &mutexes[id];

    // Initialize the mutex data structure
    memset(mutex, 0, sizeof(mutex_t));
    queue_init(&mutex->wait_queue);
    mutex->allocated = 1;

    return id;
}

/**
 * Frees the specified mutex
 * @param id - the mutex id
 * @return 0 on success, -1 on error
 */
int kmutex_free(int id) {
    mutex_t *mutex;

    // Ensure that the id is within the valid range
    if (id < 0 || id >= MUTEX_MAX) {
        return -1;
    }

    // Pointer to the mutex table entry
    mutex = &mutexes[id];

    // If the mutex is locked, prevent it from being freed
    if (mutex->lock_count > 0) {
        return -1;
    }

    // Add the id back into the mutex queue to be re-used later
    if (queue_in(&mutex_queue, id) != 0) {
        return -1;
    }

    // Clear the memory for the data structure
    memset(mutex, 0, sizeof(mutex_t));

    return 0;
}

/**
 * Locks the specified mutex
 * @param id - the mutex id
 * @return -1 on error, otherwise the current lock count
 */
int kmutex_lock(int id) {
    mutex_t *mutex;

    if (id < 0 || id > MUTEX_MAX) {
        return -1;
    }

    mutex = &mutexes[id];

    // If the mutex is already locked
    //   1. Remove the process from the scheduler, allow another
    //      process to be scheduled
    //   2. Add the process to the mutex wait queue (so it can take
    //      the mutex when it is unlocked)
    //   3. Set the state to WAITING
    // If the mutex is not locked
    //   1. set the mutex owner to the current process
    // Increment the lock count

    if (mutex->lock_count > 0) {
        if (queue_in(&mutex->wait_queue, current->pid) != 0) {
            panic_warn("Unable to add process to wait queue");
        }

        current->state = WAITING;
        scheduler_remove(current);

        current = NULL;
    } else {
        mutex->owner = current;
    }

    return mutex->lock_count++;
}

/**
 * Unlocks the specified mutex
 * @param id - the mutex id
 * @return -1 on error, otherwise the current lock count
 */
int kmutex_unlock(int id) {
    mutex_t *mutex;
    proc_t *proc;
    int pid = -1;
    int i = 0;


    if (id < 0 || id >= MUTEX_MAX) {
        return -1;
    }

    mutex = &mutexes[id];

    // If the mutex is not locked, there is nothing to do
    // Decrement the lock count
    // If there are no more locks held:
    //    1. clear the owner of the mutex
    // If there are still locks held:
    //    1. Obtain a process from the mutex wait queue
    //    2. Add the process back to the scheduler
    //    3. Set the state to RUNNING

    if (mutex->lock_count == 0) {
        return 0;
    }

    if (mutex->owner != current) {
        panic_warn("Only the owner may unlock the mutex");
        return -1;
    }

    // Decrement the lock count
    mutex->lock_count--;

    if (mutex->lock_count == 0) {
        // No more owner as all locks have been released
        mutex->owner = NULL;
    } else {
        if (queue_out(&mutex->wait_queue, &pid) != 0) {
            panic_warn("No processes in the mutex queue");
            return -1;
        }

        for (i = 0; i < PROC_MAX; i++) {
            proc = &proc_table[i];

            if (proc->pid == pid) {
                scheduler_add(proc);
                mutex->owner = proc;
                proc->state = RUNNING;

                break;
            }
        }
    }

    return mutex->lock_count;
}
