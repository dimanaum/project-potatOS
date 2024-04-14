/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Process Handling
 */

#include <spede/string.h>
#include <spede/machine/proc_reg.h>

#include "kernel.h"
#include "kproc.h"
#include "kutil.h"
#include "scheduler.h"

#include "queue.h"

/**
 * Initialize the scheduler
 */
void scheduler_init() {
    queue_init(&run_queue);
}

/**
 * Process scheduler
 */
void scheduler_run() {
    int i;
    int pid;

    proc_t *proc;

    // Ensure that processes not in the active state aren't still scheduled
    if (current && current->state != ACTIVE) {
        current = NULL;
    }

    // Check if we have a current/active process
    if (current) {
        // Check if the current process has exceeded it's time slice
        if (current->active_time >= PROC_TIMESLICE) {
            // Reset the active time
            current->active_time = 0;

            if (current->pid != 0) {
                // Add the process to the scheuler
                scheduler_add(current);
            } else {
                current->state = RUNNING;
            }

            // Unschedule the current process
            current = NULL;
        }
    }

    // Check if we have any processes that need to wake up
    for (i = 0; i < PROC_MAX; i++) {
        // Local pointer
        proc = &proc_table[i];

        // Is the process sleeping?
        if (proc->state == SLEEPING) {
            // Is the process ready to wake up yet?
            if (proc->wake_time <= system_time) {
                // Clear the wake time, active time and add the process to the scheduler
                proc->active_time = 0;
                proc->wake_time = 0;
                scheduler_add(proc);
            }
        }
    }

    // Check if we have a process scheduled or not
    if (!current) {
        // Get the proces id from the run queue
        if (queue_out(&run_queue, &pid) != 0) {
            // default to process id 0 (idle task)
            pid = 0;
        }

        // Look up the process for the given process id
        for (i = 0; i < PROC_MAX; i++) {
            if (proc_table[i].pid == pid) {
                current = &proc_table[i];
                break;
            }
        }
    }

    // Make sure we have a valid process at this point
    if (!current) {
        panic("Unable to schedule a process!");
    }

    // Ensure that the process state is correct
    current->state = ACTIVE;
}

void scheduler_add(proc_t *proc) {
    if (!proc) {
        panic("Invalid process!");
    }

    if (queue_in(&run_queue, proc->pid) != 0) {
        panic("Unable to add the process to the scheduler");
    }

    proc->state = RUNNING;
}

void scheduler_remove(proc_t *proc) {
    int i;
    int pid;

    if (!proc) {
        panic("Invalid process!");
    }

    for (i = 0; i < run_queue.size; i++) {
        if (queue_out(&run_queue, &pid) != 0) {
            panic("Unable to queue out the process entry");
        }

        if (proc->pid == pid) {
            // Found the process
            break;
        }

        // Add the item back to the run queue
        if (queue_in(&run_queue, pid) != 0) {
            panic("Unable to queue process back to the run queue");
        }
    }
}
