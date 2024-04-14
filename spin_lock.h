/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Simple spinlock
 */
#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H

/**
 * Takes a lock
 * @param *lock pointer to the lock
 * @return 0 on success, -1 on error
 */
int spin_lock(int *lock);

/**
 * Frees a lock
 * @param *lock pointer to the lock
 * @return 0 on success, -1 on error
 */
int spin_unlock(int *lock);

#endif
