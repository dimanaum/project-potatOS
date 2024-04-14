/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Utilities
 */

#ifndef KUTIL_H
#define KUTIL_H


/**
 * Function declarations
 */

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a panic message on the host console
 *   - Triggers a breakpiont (if running through GDB)
 *   - aborts/exits
 * @param msg   message to be displayed
 */
void panic(char *msg, ...);

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a warning message on the host console
 *   - Triggers a breakpoint (if running through GDB)
 * @param msg   message to be displayed
 */
void panic_warn(char *msg, ...);


/**
 * Dumps the process list
 */
void kernel_dump();
#endif
