/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Interrupt handling functions
 */
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

// Interrupt definitions
#define TIMER_INTR 0x20     // IRQ 0 (Timer)
#define SYSCALL_INTR 0x80   // System call Interrupt

#ifndef ASSEMBLER
/**
 * Configures and enables interrupts
 *   - Adds entries to the IDT
 *   - Enables IRQs in the PIC
 */
void interrupts_init();

/**
 * Handles the specified interrupt request
 * @param irq - interrupt number
 */
void irq_handler(int irq);

/**
 * Enables the specified irq
 * @param irq - interrupt number
 */
void irq_enable(int irq);

/**
 * Disables the specified IRQ on the PIC
 *
 * @param irq - IRQ that should be disabled
 */
void irq_disable(int irq);

/**
 * Queries if the given IRQ is enabled on the PIC
 * @param irq - IRQ to check
 * @return - 1 if enabled, 0 if disabled
 */
int irq_enabled(int irq);

/**
 * Dismisses the specified irq
 * @param irq - interrupt number
 */
void irq_dismiss(int irq);

#endif
#endif
