/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * OS Main Entry Point
 */

// SPEDE Includes
#include <spede/flames.h>
#include <spede/stdio.h>

#include "kernel.h"

int main() {
    cons_clear();

    kernel_init();

    // Should never hit this!
    return 0;
}
