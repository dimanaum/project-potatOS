/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Internal Kernel APIs
 */
#include <spede/flames.h>
#include <spede/stdio.h>

#include "kernel.h"

#include "vga.h"

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a panic message on the target console
 *   - Triggers a breakpiont (if running through GDB)
 *   - aborts/exits
 * @param msg   the message to display
 */
void panic(char *msg, ...) {
    va_list args;

    printf("PANIC: ");

    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);

    breakpoint();
    abort();
}

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a warning message on the target console
 *   - Triggers a breakpoint (if running through GDB)
 * @param msg   the message to display
 */
void panic_warn(char *msg, ...) {
    va_list args;

    printf("WARN: ");

    va_start(args, msg);
    printf(msg, args);
    va_end(args);

    breakpoint();
}

