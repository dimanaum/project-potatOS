/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Internal Kernel APIs and Data structures
 */
#ifndef KERNEL_H
#define KERNEL_H

// Kernel stack size
#define KSTACK_SIZE 16384

// Kernel code segment
#define KCODE 0x08

// Kernel data segment
#define KDATA 0x10


#ifndef ASSEMBLER

#include "mbox.h"
#include "kmutex.h"
#include "kproc.h"
#include "queue.h"

/**
 * Kernel data structures
 */

// System time
extern int system_time;

// Next available process id
extern int next_pid;

// Current process
extern proc_t *current;

// Process table
extern proc_t proc_table[PROC_MAX];

// runtime stacks of processes
extern char proc_stack[PROC_MAX][PROC_STACK_SIZE];

// Available process table entries
extern queue_t proc_queue;

// Running processes
extern queue_t run_queue;

// Mutex data structures
extern mutex_t mutexes[MUTEX_MAX];
extern queue_t mutex_queue;

// Mailbox data structures
extern mbox_t mailboxes[MBOX_MAX];

/**
 * Function declarations
 */

/*
 * Kernel initialization
 * Initializes all kernel data structures and variables
 */
void kernel_init();

/*
 * Kernel idle task
 * Should never return
 */
void kernel_idle();

/**
 * Implementation in kernel_entry.S
 * Performs the context switch out of the kernel
 */
void kernel_context_switch(trapframe_t *trapframe);

/* Defined in kernel_entry.S */
__BEGIN_DECLS

// Context switcher
extern void kernel_context_switch();

__END_DECLS
#endif
#endif
