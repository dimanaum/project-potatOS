/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Interrupt handling functions
 */

#include <spede/stdio.h>
#include <spede/machine/io.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>

#include "kisr.h"
#include "kutil.h"
#include "interrupts.h"

#define PIC1_BASE   0x20            // base address for PIC primary controller
#define PIC2_BASE   0xa0            // base address for PIC secondary controller
#define PIC1_CMD    PIC1_BASE       // address for issuing commands to PIC1
#define PIC1_DATA   (PIC1_BASE+1)   // address for setting data for PIC1
#define PIC2_CMD    PIC2_BASE       // address for issuing commands to PIC2
#define PIC2_DATA   (PIC2_BASE+1)   // address for setting data for PIC2

#define PIC_EOI     0x20            // PIC End-of-Interrupt command

// Interrupt descriptor table
struct i386_gate *idt_p;


// Forward Declarations
void idt_entry_add(int entry_num, void (*func_ptr)());


/**
 * Interrupt Descriptor Table initialization
 * This adds entries to the IDT and enables any IRQs in the PIC
 */
void interrupts_init() {
    printf("Initializing Interrupts\n");

    // Get the IDT base address
    idt_p = get_idt_base();

    // Add an entry for each interrupt into the IDT
    idt_entry_add(TIMER_INTR, kisr_entry_timer);

    idt_entry_add(SYSCALL_INTR, kisr_entry_syscall);

    // Enable IRQs in the PIC
    irq_enable(TIMER_INTR);
}

/**
 * Handles the specified interrupt request
 * @param irq - interrupt number
 */
void irq_handler(int irq) {
    switch(irq) {
        case TIMER_INTR:
            kisr_timer();
            break;

        case SYSCALL_INTR:
            kisr_syscall();
            break;

        default:
            panic("Unhandled interrupt: %d\n", irq);
    }
}

/**
 * Adds an entry to the IDT
 * @param  entry_num - interrupt/entry number
 * @param  func_ptr  - pointer to the function (interrupt service routine) when
 *                     the interrupt is triggered.
 */
void idt_entry_add(int entry_num, void (*func_ptr)()) {
    struct i386_gate *gateptr;

    // Ensure that we have a valid entry
    if (entry_num < 0 || entry_num > 255) {
        panic("Invalid interrupt %d", entry_num);
    }

    if (func_ptr == NULL) {
        panic("Invalid function pointer");
    }

    // Obtain the gate pointer for the entry
    gateptr = &idt_p[entry_num];

    // Fill the gate
    fill_gate(gateptr, (int)func_ptr, get_cs(), ACC_INTR_GATE, 0);
}

/**
 * Enables the specified IRQ on the PIC
 *
 * @param irq - IRQ that should be enabled
 * @note IRQs > 0xf will be remapped
 */
void irq_enable(int irq) {
    int port = PIC1_DATA;
    int mask = 0x60;

    // Isolate only the first nibble; handles remapping
    irq &= 0xf;

    // Select the secondary PIC if the IRQ is associated with it
    if (irq >= 0x8) {
        port = PIC2_DATA;
        irq -= 0x8;
    }

    // Read the current mask
    mask = inportb(port);

    // Set the IRQ
    mask &= ~(1 << irq);

    outportb(port, 0xfe);

    // Write the mask out to the PIC
    outportb(port, mask);
}

/**
 * Disables the specified IRQ on the PIC
 *
 * @param irq - IRQ that should be disabled
 */
void irq_disable(int irq) {
    int port = PIC1_DATA;
    int mask = 0x60;

    irq &= 0xf;

    if (irq >= 0x8) {
        port = PIC2_DATA;
        irq -= 0x8;
    }

    // Read the current mask
    mask = inportb(port);

    // Set the IRQ
    mask = 0xff & (mask ^ (1 << irq));

    // Write the mask out to the PIC
    outportb(port, mask);
}

/**
 * Queries if the given IRQ is enabled on the PIC
 *
 * @param irq - IRQ to check
 * @return - 1 if enabled, 0 if disabled
 */
int irq_enabled(int irq) {
    int port = PIC1_DATA;
    int mask = 0x0;

    irq &= 0xf;

    if (irq >= 0x8) {
        port = PIC2_DATA;
        irq -= 0x8;
    }

    // Read the current mask
    mask = inportb(port);

    return (mask & (1 << irq)) ? 0 : 1;
}

/**
 * Dismisses an interrupt by sending the EOI command to the appropriate
 * PIC device(s).
 *
 * @param irq - IRQ that we are dismissing the interrupt
 * @note IRQs > 0xf will be remapped
 * @note If the IRQ is assosciated with the secondary PIC, then the
 *       EOI command must be issued to both
 */
void irq_dismiss(int irq) {
    // Isolate only the first nibble; handles remapping
    irq &= 0xf;

    if (irq >= 0x8) {
        // Send EOI to the secondary PIC
        outportb(PIC2_CMD, PIC_EOI);
    }

    // Send EOI to the primary PIC
    outportb(PIC1_CMD, PIC_EOI);
}

