/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel Interrupt Service Routines
 */
#ifndef KISR_H
#define KISR_H
#include <spede/sys/cdefs.h>

#ifndef ASSEMBLER
#include "interrupts.h"

/**
 * Function declarations
 */

// Timer ISR
void kisr_timer();

// System call ISR
void kisr_syscall();


/* Defined in kisr_entry.S */
__BEGIN_DECLS

// Kernel interrupt entries
extern void kisr_entry_timer();
extern void kisr_entry_syscall();

__END_DECLS
#endif
#endif
