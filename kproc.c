/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Process Handling
 */

#include <spede/stdio.h>
#include <spede/string.h>
#include <spede/machine/proc_reg.h>

#include "kernel.h"
#include "kutil.h"
#include "kproc.h"
#include "scheduler.h"


/**
 * Start a new process
 * @param proc_name The process title
 * @param proc_ptr  function pointer for the process
 * @param queue     the run queue in which this process belongs
 * @return process id of the executed process
 */
int kproc_exec(void *proc_ptr, char *proc_name) {
    int proc_entry;
    proc_t *proc;

    // Ensure that valid parameters have been specified
    if (proc_name == NULL) {
        panic("Invalid process title\n");
    }

    if (proc_ptr == NULL) {
        panic("Invalid function pointer");
    }

    // Allocate the PCB entry for the process
    if (queue_out(&proc_queue, &proc_entry) != 0) {
        panic_warn("Unable to allocate a process entry!");
        return -1;
    }

    // Allocate the process table entry
    proc = &proc_table[proc_entry];

    // Initialize the PCB entry for the process
    memset(proc, 0, sizeof(proc_t));

    // Point the stack to the process stack
    proc->stack = proc_stack[proc_entry];

    // Set the process state to RUNNING
    // Initialize other process control block variables to default values
    proc->pid         = next_pid++;
    proc->state       = RUNNING;
    proc->active_time = 0;
    proc->cpu_time    = 0;
    proc->start_time  = system_time;

    // Copy the process name to the PCB
    strncpy(proc->name, proc_name, PROC_NAME_LEN);

    // Ensure the stack for the process is cleared
    memset(proc->stack, 0, PROC_STACK_SIZE);

    // Allocate the trapframe data
    proc->trapframe = (trapframe_t *)(&proc->stack[PROC_STACK_SIZE - sizeof(trapframe_t)]);

    // Set the instruction pointer in the trapframe
    proc->trapframe->eip = (unsigned int)proc_ptr;

    // Set INTR flag
    proc->trapframe->eflags = EF_DEFAULT_VALUE | EF_INTR;

    // Set each segment in the trapframe
    proc->trapframe->cs = get_cs();
    proc->trapframe->ds = get_ds();
    proc->trapframe->es = get_es();
    proc->trapframe->fs = get_fs();
    proc->trapframe->gs = get_gs();

    // Add the process to the run queue
    scheduler_add(proc);

    printf("Executed process %s (%d) entry=%d\n", proc->name, proc->pid, proc_entry);

    return proc->pid;
}

/**
 * Exit the currently running process
 */
void kproc_exit(proc_t *proc) {
    int i;

    if (proc == NULL) {
        panic("Invalid process!");
        return;
    }

    if (proc->pid == 0) {
        printf("Cannot exit the idle task\n");
        return;
    }

    // Remove the process from the scheduler
    scheduler_remove(proc);

    // Clean up the process table for the process
    for (i = 0; i < PROC_MAX; i++) {
        if (proc->pid == proc_table[i].pid) {
            printf("Exiting process %s (%d) entry=%d\n", proc->name, proc->pid, i);

            // Clear out the process stack
            memset(proc->stack, 0, PROC_STACK_SIZE);

            // Clear out the process control block data
            memset(proc, 0, sizeof(proc_t));

            // Add the proc entry back to the proc queue
            queue_in(&proc_queue, i);

            // if the current process is being exited, make sure we handle it
            if (current == proc) {
                current = NULL;
            }

            // Return as there is nothing else to do
            return;
        }
    }

    // If we got here, something bad happened
    panic("Unable to exit process!");
}

