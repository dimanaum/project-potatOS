/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * General utility functions
 */

#ifndef UTIL_H
#define UTIL_H

#define IO_DELAY_LOOP 1666666

/**
 * Creates a busy delay for the specified number of milliseconds
 * @param msec - Number of milliseconds to delay
 */
void mdelay(int msec);


/**
 * Creates a busy delay for the specified number of seconds
 * @param secs - Number of seconds to delay
 */
void delay(int secs);

#endif
