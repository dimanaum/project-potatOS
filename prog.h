/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * User/Test programs
 */
#ifndef PROG_H
#define PROG_H

/**
 * User program to initialize/execute other programs at startup
 * Will never return/exit. Once programs are executed, will loop forever
 * and yield the CPU.
 */
void prog_init();

/**
 * Test program to exercise various system calls
 *
 * Iterates over alternating sleep or busy delays and then finally exits.
 * Exercises a variety of system calls to make sure they are functional
 */
void prog_test();


/**
 * Test program to exercise message sending
 */
void prog_producer();

/**
 * Test program to exercise message receiving
 */
void prog_consumer();
#endif
