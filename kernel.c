/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Main Kernel Entry point
 */

#include <spede/flames.h>
#include <spede/stdio.h>
#include <spede/string.h>

#include "interrupts.h"
#include "kernel.h"
#include "kutil.h"
#include "kproc.h"
#include "scheduler.h"
#include "queue.h"
#include "vga.h"
#include "prog.h"
#include "kmutex.h"

/**
 * Kernel data structures and variables
 * Externed via kernel.h
 */

// Current system time
int system_time;

// Next process id
int next_pid;

// Current process
proc_t *current;

// Process queue
queue_t proc_queue;

// Run queue
queue_t run_queue;

// Process table
proc_t proc_table[PROC_MAX];

// Process stacks
char proc_stack[PROC_MAX][PROC_STACK_SIZE];


int display_kernel_stats = 1;
void kernel_stats();

/**
 * Kernel Initialization
 * Initializes all kernel data structures and variables
 */
void kernel_init() {
    int i;

    printf("Initializing kernel data structures\n");

    // Initialize system time
    system_time = 0;

    // Initialize next process id
    next_pid = 0;

    // Initialize the process table
    memset(proc_table, 0, sizeof(proc_table));

    // Initialize the process stacks
    memset(proc_stack, 0, sizeof(proc_stack));

    // Initialize the process queue
    queue_init(&proc_queue);

    for (i = 0; i < PROC_MAX; i++) {
        if (queue_in(&proc_queue, i) != 0) {
            panic("Unable to initialize process queue for process entry %d", i);
        }
    }

    queue_init(&mutex_queue);
    for (i = 0; i < MUTEX_MAX; i++) {
        if (queue_in(&mutex_queue, i) != 0) {
            panic("Unable to initialize mutex queue for mutex id %d", i);
        }
    }

    // Initialize interrupts
    interrupts_init();

    // Initialize the scheduler (and run queue)
    scheduler_init();

    // initialize mailbox
    mbox_init();

    // Launch the idle task
    kproc_exec(&kernel_idle, "idle task");

    // Launch the init task
    kproc_exec(&prog_init, "init task");

    // Run the scheduler
    printf("Running the scheduler\n");
    scheduler_run();

    // Trigger the initial context switch
    printf("Performing initial context switch\n");
    kernel_context_switch(current->trapframe);
}

void kernel_idle() {
    while(1) {
        asm("sti"); // Ensure that interrupts are enabled
        asm("hlt"); // Halt the CPU from executing additional instructions
    }
}

void kernel_run(trapframe_t *trapframe) {
    char kbd_ch;

    // Save the trapframe of the currently running process
    current->trapframe = trapframe;

    // Run the interrupt handler
    irq_handler(trapframe->interrupt);

    if (cons_kbhit()) {
        /* Read the character from the target keyboard buffer */
        kbd_ch = cons_getchar();

        switch (kbd_ch) {
            case 'b':
                breakpoint();
                break;

            case 'n':
                kproc_exec(&prog_test, "test task");
                break;

            case 's':
                display_kernel_stats = display_kernel_stats ? 0 : 1;
                cons_clear();
                break;

            case 'x':
                kproc_exit(current);
                break;

            case 'q':
                cons_printf("Exiting!!!");
                exit(0);
                break;

            case 'p':
                kproc_exec(&prog_producer, "Producer Program");
                break;
            
            case 'c':
                kproc_exec(&prog_consumer, "Consumer Program");
                break;

            default:
                break;
        }
    }

    // Display the kernel stats on the console
    if (display_kernel_stats) {
        kernel_stats();
    }

    // Run the scheduler
    scheduler_run();

    // Perform the context switch out of the kernel
    kernel_context_switch(current->trapframe);
}

void kernel_stats() {
    int i;

    proc_t *proc;

    int row;

    char buf[80];
    char state;

    int display_attr;
    int default_attr;
    int header_attr;
    int active_attr;
    int running_attr;
    int sleeping_attr;
    int waiting_attr;
    int unknown_attr;

    default_attr = vga_attr(VGA_COLOR_BLACK, VGA_COLOR_WHITE, 0);
    header_attr  = vga_attr(VGA_COLOR_CYAN, VGA_COLOR_BLACK, 0);
    active_attr  = vga_attr(VGA_COLOR_GREEN, VGA_COLOR_WHITE, 0);
    running_attr = vga_attr(VGA_COLOR_BLUE, VGA_COLOR_WHITE, 0);
    sleeping_attr = vga_attr(VGA_COLOR_BLACK, VGA_COLOR_YELLOW, 0);
    waiting_attr = vga_attr(VGA_COLOR_BLACK, VGA_COLOR_CYAN, 0);
    unknown_attr = vga_attr(VGA_COLOR_RED, VGA_COLOR_WHITE, 1);


    memset(buf, 0, sizeof(buf));
    memset(buf, ' ', sizeof(buf)-1);

    cons_clear();

    row = 1;

    vga_print_str(0, 0, header_attr, buf);
    vga_print_str(0, 0, header_attr, "Entry    PID   State    Time   Command");

    for (i = 0; i < PROC_MAX; i++) {
        proc = &proc_table[i];

        if (proc->state == NONE) {
            continue;
        }

        if (proc->state == ACTIVE) {
            state = 'A';
            display_attr = active_attr;
        } else if (proc->state == RUNNING) {
            state = 'R';
            display_attr = default_attr;
        } else if (proc->state == SLEEPING) {
            state = 'S';
            display_attr = sleeping_attr;
        } else if (proc->state == WAITING) {
            state = 'W';
            display_attr = waiting_attr;
        } else {
            state = '?';
            display_attr = unknown_attr;
        }

        snprintf(buf, 80, "%5d  %5d  %4c  %8d   %s",
                 i, proc->pid, state, proc->cpu_time, proc->name);
        vga_print_str(row++, 0, display_attr, buf);
    }

    snprintf(buf, 80, "%d", system_time);
    vga_print_str(0, VGA_COL_MAX-8, header_attr, "Sys Time");
    vga_print_str(1, VGA_COL_MAX-strlen(buf), default_attr, buf);

    row = 1;

    vga_print_char(0, 48, header_attr, 186);
    vga_print_char(0, 70, header_attr, 186);

    vga_print_str(0, 50, header_attr, "Mutex  Owner  Locks");

    for (i = 0; i < MUTEX_MAX; i++) {
        if (mutexes[i].allocated != 1) {
            continue;
        }

        snprintf(buf, 80, "%5d  %5d  %5d", i, mutexes[i].owner ? mutexes[i].owner->pid : -1, mutexes[i].lock_count);
        vga_print_str(row++, 50, default_attr, buf);
    }
}

