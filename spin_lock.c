/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Simple spinlock
 */

#include "spin_lock.h"

/**
 * Locks the spinlock
 * @param *lock pointer to the lock
 * @return 0 on success, -1 on error
 */
int spin_lock(int *lock) {
    if (!lock) {
        return -1;
    }

    // Loop forever (spin) while the lock is set
    while(*lock != 0);

    // Once the lock is unlocked, immediately lock it
    *lock = 1;

    return 0;
}

/**
 * Unlocks the spinlock
 * @param *lock pointer to the lock
 * @return 0 on success, -1 on error
 */
int spin_unlock(int *lock) {
    if (!lock) {
        return -1;
    }

    // Set the lock to 0 to unlock it
    *lock = 0;

    return 0;
}
