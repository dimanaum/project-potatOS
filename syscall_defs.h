/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Common system call definitions
 */
#ifndef SYSCALL_DEFS_H
#define SYSCALL_DEFS_H

// Syscall definitions
typedef enum {
    SYSCALL_SYS_GET_TIME,
    SYSCALL_PROC_EXEC,
    SYSCALL_PROC_EXIT,
    SYSCALL_PROC_GET_NAME,
    SYSCALL_PROC_GET_PID,
    SYSCALL_SLEEP,
    SYSCALL_YIELD,
    SYSCALL_MUTEX_ALLOC,
    SYSCALL_MUTEX_FREE,
    SYSCALL_MUTEX_LOCK,
    SYSCALL_MUTEX_UNLOCK,
    SYSCALL_MSG_SEND,
    SYSCALL_MSG_RECV
} syscall_t;

#endif
