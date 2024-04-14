/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Process Scheduler
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

/**
 * Initializes the scheduler
 */
void scheduler_init();

/**
 * Executes the scheduler
 */
void scheduler_run();

/**
 * Adds a process to the scheduler
 * @param proc - pointer to the process entry
 */
void scheduler_add(proc_t *proc);

/**
 * Removes a process from the scheduler
 * @param proc - pointer to the process entry
 */
void scheduler_remove(proc_t *proc);

#endif
