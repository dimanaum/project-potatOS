/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Process Handling
 */
#ifndef KPROC_H
#define KPROC_H

#include "trapframe.h"

#define PROC_MAX        24   // maximum number of processes to support
#define PROC_NAME_LEN   32   // Maximum length of a process name
#define PROC_STACK_SIZE 8192 // Process stack size
#define PROC_TIMESLICE  5    // Number of ticks a process can execute at a time

// Process States
typedef enum {
    NONE,       // Process has no state
    RUNNING,    // Process is running (in run queue)
    ACTIVE,     // Process is active (scheduled/executing)
    SLEEPING,   // Process is sleeping (unscheduled)
    WAITING     // Process is waiting (unscheduled)
} state_t;

typedef struct proc_t {
    int pid;                  // Process id
    int state;                // Process state

    char name[PROC_NAME_LEN]; // Process name

    int start_time;           // Time started
    int cpu_time;             // Total CPU time
    int active_time;          // Current active time
    int wake_time;            // Time when process should wake from sleeping

    char *stack;              // Pointer to the stack

    trapframe_t *trapframe;   // Pointer to the trapframe
} proc_t;

/**
 * Process functions
 */

/**
 * Executes a new process
 * @param proc_ptr - address of process to execute
 * @param proc_name - "friendly" process name
 * @return process id of the created process
 */
int kproc_exec(void *proc_ptr, char *proc_name);

/**
 * Exits a process
 * @param proc - process entry
 */
void kproc_exit(proc_t *proc);

#endif
