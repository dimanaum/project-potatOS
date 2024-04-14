/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Interrupt Service Routines
 */

#include <spede/machine/io.h>

#include "interrupts.h"
#include "kernel.h"
#include "kisr.h"
#include "ksyscall.h"
#include "kutil.h"

/**
 * Kernel Interrupt Service Routine: Timer (IRQ 0)
 */
void kisr_timer() {
    // Increment the system time
    system_time++;

    // Increment the current process' active time
    current->active_time++;

    // Increment the current process' cpu time
    current->cpu_time++;

    irq_dismiss(TIMER_INTR);
}

/**
 * Kernel Interrupt Service Routine: System Call
 * Runs the ksyscall_handler() function and passes in
 * parameters sent from the caller via the trapframe.
 * Return code/value to the caller set via the trapframe.
 */
void kisr_syscall() {
    int rc;

    if (!current || !current->trapframe) {
        panic("No current process!");
    }

    rc = ksyscall_handler(
        current->trapframe->eax,
        current->trapframe->ebx,
        current->trapframe->ecx,
        current->trapframe->edx);

    if (current && current->trapframe) {
        current->trapframe->eax = rc;
    }
}
