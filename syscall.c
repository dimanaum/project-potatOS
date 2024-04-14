/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * System call APIs
 */
#include "syscall.h"


/*
 * Puts the current process to sleep for the specified number of seconds
 * @param seconds - number of seconds the process should sleep
 */
void sleep(int seconds) {
    asm("movl %0, %%eax;"
        "movl %1, %%ebx;"
        "int $0x80;"
        :
        : "g"(SYSCALL_SLEEP), "g"(seconds)
        : "%eax", "%ebx");
}


/*
 * Allows a process to indicate that it can relinquish control of the CPU
 */
void yield(void) {
    asm("movl %0, %%eax;"
        "int $0x80;"
        :
        : "g"(SYSCALL_YIELD)
        : "%eax");
}


/*
 * Gets the current system time (in seconds)
 * @return system time in seconds
 */
int sys_get_time() {
    int time = -1;

    asm("movl %1, %%eax;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(time)
        : "g"(SYSCALL_SYS_GET_TIME)
        : "%eax");

    return time;
}


/*
 * Executes a new process
 */
int proc_exec(void *ptr, char *name) {
    int pid;

    asm("movl %1, %%eax;"
        "movl %2, %%ebx;"
        "movl %3, %%ecx;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(pid)
        : "g"(SYSCALL_PROC_EXEC), "g"(ptr), "g"(name)
        : "%eax");

    return pid;
}


/*
 * Exits the current process
 */
void proc_exit() {
    asm("movl %0, %%eax;"
        "int $0x80;"
        :
        : "g"(SYSCALL_PROC_EXIT)
        : "%eax");
}


/*
 * Gets the current process' id
 * @return process id
 */
int proc_get_pid() {
    int pid = -1;

    asm("movl %1, %%eax;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(pid)
        : "g"(SYSCALL_PROC_GET_PID)
        : "%eax");

    return pid;
}


/*
 * Gets the current process' name
 * @param name - pointer to a character buffer where the name will be copied
 * @return 0 on success, -1 or other non-zero value on error
 */
int proc_get_name(char *name) {
    int rc = -1;

    asm("movl %1, %%eax;"
        "movl %2, %%ebx;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(rc)
        : "g"(SYSCALL_PROC_GET_NAME), "g"(name)
        : "%eax", "%ebx");

    return rc;
}


/**
 * Allocates a mutex from the kernel
 * @return -1 on error, all other values indicate the mutex id
 */
int mutex_alloc(void) {
    int rc = -1;

    asm("movl %1, %%eax;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(rc)
        : "g"(SYSCALL_MUTEX_ALLOC)
        : "%eax", "%ebx");

    return rc;
}

/**
 * Frees a mutex
 * @return -1 on error, 0 on sucecss
 */
int mutex_free(int mutex) {
    int rc = -1;

    asm("movl %1, %%eax;"
        "movl %2, %%ebx;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(rc)
        : "g"(SYSCALL_MUTEX_FREE), "g"(mutex)
        : "%eax", "%ebx");

    return rc;
}

/**
 * Locks the mutex
 * @param *mutex - pointer to the mutex identifer
 * @return -1 on error, 0 on sucecss
 * @note If the mutex is already locked, process will block/wait.
 */
int mutex_lock(int mutex) {
    int rc = -1;

    asm("movl %1, %%eax;"
        "movl %2, %%ebx;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(rc)
        : "g"(SYSCALL_MUTEX_LOCK), "g"(mutex)
        : "%eax", "%ebx");

    return rc;
}

/**
 * Unlocks the mutex
 * @return -1 on error, 0 on sucecss
 * @param *mutex - pointer to the mutex identifier
 */
int mutex_unlock(int mutex) {
    int rc = -1;

    asm("movl %1, %%eax;"
        "movl %2, %%ebx;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(rc)
        : "g"(SYSCALL_MUTEX_UNLOCK), "g"(mutex)
        : "%eax", "%ebx");

    return rc;
}

/**
 * Sends a message to the specified mailbox
 * @param mbox - Mailbox number to send to
 * @param msg - Pointer to the message data structure
 * @return -1 on error, 0 on success
 */
int msg_send(int mbox, msg_t *msg) {
    int rc = -1;

    // Trigger the system call
    asm("movl %1, %%eax;"
        "movl %2, %%ebx;"
        "movl %3, %%ecx;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(rc)
        : "g"(SYSCALL_MSG_SEND), "g"(mbox), "g"(msg)
        : "%eax");

    return rc;
}

/**
 * Receives a message from the specified mailbox
 * This call is blocking; if no message exists in the mailbox, the system
 * call will block (process moves to WAITING state)
 *
 * @param mbox - Mailbox number to send to
 * @param msg - Pointer to the message data structure
 * @return -1 on error, 0 on success
 */
int msg_recv(int mbox, msg_t *msg) {
    int rc = -1;

    // Trigger the system call
    asm("movl %1, %%eax;"
        "movl %2, %%ebx;"
        "movl %3, %%ecx;"
        "int $0x80;"
        "movl %%eax, %0;"
        : "=g"(rc)
        : "g"(SYSCALL_MSG_RECV), "g"(mbox), "g"(msg)
        : "%eax");

    return rc;
}
