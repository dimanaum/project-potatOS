/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * General utility functions
 */
#include <spede/flames.h>
#include "util.h"

/**
 * Creates a busy delay for the specified number of milliseconds
 * @param msec - Number of milliseconds to delay
 */
void mdelay(int msec) {
    int i;

    for (i = 0; i < IO_DELAY_LOOP; i++) {
        IO_DELAY();
    }
}


/**
 * Creates a busy delay for the specified number of seconds
 * @param secs - Number of seconds to delay
 */
void delay(int secs) {
    mdelay(secs * 1000);
}

