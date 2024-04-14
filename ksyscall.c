/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * Kernel System Call Handlers
 */
#include <spede/time.h>
#include <spede/string.h>
#include <spede/stdio.h>

#include "kernel.h"
#include "kproc.h"
#include "ksyscall.h"
#include "kutil.h"
#include "kmutex.h"
#include "queue.h"
#include "scheduler.h"
#include "mbox.h"

/**
 * System call handler
 * Dispatches system calls to the function associate with the specified system call
 * @param syscall - system call number (defined in syscall_defs.h)
 * @param arg1 - system call argument 1
 * @param arg2 - system call argument 2
 * @param arg3 - system call argument 3
 * @return return value from the specified system call function
 */
int ksyscall_handler(int syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    // Default return value
    int rc = -1;

    // Dispatch to the appropriate function
    // Cast parameters as necessary
    switch (syscall) {
        case SYSCALL_SLEEP:
            rc = ksyscall_sleep((int)arg1);
            break;

        case SYSCALL_YIELD:
            rc = ksyscall_yield();
            break;

        case SYSCALL_PROC_EXEC:
            rc = ksyscall_proc_exec((char *)arg1, (void *)arg2);
            break;

        case SYSCALL_PROC_EXIT:
            rc = ksyscall_proc_exit();
            break;

        case SYSCALL_SYS_GET_TIME:
            rc = ksyscall_sys_get_time();
            break;

        case SYSCALL_PROC_GET_PID:
            rc = ksyscall_proc_get_pid();
            break;

        case SYSCALL_PROC_GET_NAME:
            rc = ksyscall_proc_get_name((char *)arg1);
            break;

        case SYSCALL_MUTEX_ALLOC:
            rc = ksyscall_mutex_alloc();
            break;

        case SYSCALL_MUTEX_FREE:
            rc = ksyscall_mutex_free((int)arg1);
            break;

        case SYSCALL_MUTEX_LOCK:
            rc = ksyscall_mutex_lock((int)arg1);
            break;

        case SYSCALL_MUTEX_UNLOCK:
            rc = ksyscall_mutex_unlock((int)arg1);
            break;

        case SYSCALL_MSG_SEND:
            rc = ksyscall_msg_send((int) arg1, (msg_t*) arg2);
            break;

        case SYSCALL_MSG_RECV:
            rc = ksyscall_msg_recv((int) arg1, (msg_t*) arg2);
            break;

        default:
            panic("Invalid system call %d!", syscall);
    }

    return rc;
}


/**
 * System call kernel handler: sys_get_time
 * Returns the current system time (in seconds) to the caller
 *
 * @return current system time (in seconds)
 */
int ksyscall_sys_get_time(void) {
    return system_time / CLK_TCK;
}

/**
 * System call kernel handler: proc_get_id
 * Returns the currently running process ID to the caller
 *
 * @return current process id
 */
int ksyscall_proc_get_pid(void) {
    if (!current) {
        panic_warn("Invalid process!");
        return -1;
    }

    return current->pid;
}

/**
 * System call kernel handler: proc_get_name
 * Copies the current process name to the buffer provided
 *
 * @param name - pointer to the buffer to copy the name
 * @return 0 on success, -1 on error
 */
int ksyscall_proc_get_name(char *name) {
    if (!current) {
        panic_warn("Invalid process!");
        return -1;
    }

    if (name && current->name) {
        // Copy the string name from the PCB to the destination
        if (strcpy(name, current->name) == name) {
            return 0;
        }
    }

    return -1;
}

/**
 * System call kernel handler: sleep
 * Immediately unschedules a process and puts it in a sleep state
 *
 * @param secs - number of seconds for the process to sleep
 * @return 0 on success, -1 on error
 */
int ksyscall_sleep(int time) {
    if (!current) {
        panic_warn("Invalid process!");
        return -1;
    }

    // Set the current state to SLEEPING
    current->state = SLEEPING;

    // Reset the current process' active time
    current->active_time = 0;

    // Set the current process' wake time
    // This should be when the scheduler will wake it up
    current->wake_time = system_time + (CLK_TCK * time);

    // Ensure that the current process will be unscheduled
    current = NULL;

    return 0;
}

/**
 * System call kernel handler: yield
 * Immediately unschedules a process
 *
 * @return 0 on success, -1 on error
 */
int ksyscall_yield(void) {
    if (!current) {
        panic_warn("Invalid process!");
        return -1;
    }

    // Set the current state back to running
    current->state = RUNNING;

    // Reset the current process' active time
    current->active_time = 0;

    // Add the current process back to the scheduer
    scheduler_add(current);

    // Ensure that the current process will be unscheduled
    current = NULL;

    return 0;
}


/**
 * System call kernel handler: proc_exec
 * Executes a new process
 *
 * @param proc_name - name of the process
 * @param proc_ptr - address of the code to execute
 * @return 0 on success, -1 on error
 */
int ksyscall_proc_exec(char *proc_name, void *proc_ptr) {
    if (!proc_name || !proc_ptr) {
        return -1;
    }

    return kproc_exec(proc_name, proc_ptr);
}


/**
 * System call kernel handler: proc_exit
 * Exit's the currently running process
 *
 * @return 0 on success, -1 on error
 */
int ksyscall_proc_exit(void) {
    if (!current) {
        panic_warn("Invalid process!");
        return -1;
    }

    kproc_exit(current);
    return 0;
}

/**
 * System call kernel handler: mutex_alloc
 * Allocates a mutex
 *
 * @return return code from kmutex_alloc
 */
int ksyscall_mutex_alloc() {
    return kmutex_alloc();
}

/**
 * System call kernel handler: mutex_free
 * Frees a mutex
 *
 * @return return code from kmutex_free
 */
int ksyscall_mutex_free(int id) {
    return kmutex_free(id);
}

/**
 * System call kernel handler: mutex_lock
 * Locks a mutex
 *
 * @return return code from kmutex_lock
 */
int ksyscall_mutex_lock(int id) {
    return kmutex_lock(id);
}

/**
 * System call kernel handler: mutex_unlock
 * Unlocks a mutex
 *
 * @return return code from kmutex_unlock
 */
int ksyscall_mutex_unlock(int id) {
    return kmutex_unlock(id);
}

/**
 * System call kernel handler: msg_send
 * Sends a message to the specified mailbox
 *
 * @return -1 on error, 0 on success
 */
int ksyscall_msg_send(int mbox, msg_t *msg) {
    proc_t *waiting_proc;
    msg_t *msg_dest;
    int pid;
    int i;
    
    // Ensure that the mailbox is valid, warn/return error if not
    if(mbox >= MBOX_MAX || mbox < 0){
        panic_warn("Invalid mailbox number");
        return -1;
    }

    // Ensure that the message pointer is valid, warn/return error if not
    if(msg == NULL){
        panic_warn("Message pointer is invalid");
        return -1;
    }

    // Set the sender of the message to the calling process' PID
    msg->sender = current->pid;

    // Set the time the message was sent (in seconds)
    msg->time_sent = system_time/CLK_TCK;

    // Enqueue the message to the mailbox
    // Treat an error here as a warning and return an error (i.e. mailbox
    // could be full)
    if(mbox_queue_in(mbox, msg) != 0){
        panic_warn("Error trying to enqueue the function.");
        return -1;
    }

    // When sending a message, if there is a process waiting to receive a
    // message, immediately handle it
    if(!queue_is_empty(&(mailboxes[mbox].wait_queue))){
        //   Dequeue the waiting process from the wait queue
        if(queue_out(&(mailboxes[mbox].wait_queue), &pid) == -1){
            panic_warn("Unable to dequeue process from wait queue");
            return -1;
        }
        
        //   Obtain the process from the PID
        for (i = 0; i < PROC_MAX; i++) {
            waiting_proc = &proc_table[i];
            if (waiting_proc->pid == pid) {
                //   Update the waiting process' state to running
                //   Add the process back to the scheduler
                scheduler_add(waiting_proc);
                waiting_proc->state = RUNNING;
                break;
            }
        }


        //   Obtain the message pointer for the receiving process (should exist on
        //   the calling process' trapframe)
        msg_dest = (msg_t *)waiting_proc->trapframe->ecx;
        //    Dequeue the message from the mailbox
        mbox_queue_out(mbox, msg_dest);
        //    Set the time the message was received (in seconds)
        msg->time_received = system_time;
    }

    // If there is no process waiting to receive a message, there is nothing
    // else to do since the message has been queued into the mailbox

    return 0;
}

/**
 * System call kernel handler: msg_recv
 * Receives a message from the specified mailbox
 * Blocking/synchronous
 *
 * @return -1 on error, 0 on success
 */
int ksyscall_msg_recv(int mbox, msg_t *msg) {
    
    // Ensure that the mailbox is valid, warn/return error if not
    if(mbox >= MBOX_MAX || mbox < 0){
        panic_warn("Invalid mailbox number");
        return -1;
    }

    // Ensure that the message pointer is valid, warn/return error if not
    if(msg == NULL){
        panic_warn("Message pointer is invalid");
        return -1;
    }

    // Check if the mailbox is empty
    if(mailboxes[mbox].size == 0){
        // If empty, we need to remove the current process from the scheduler
        // and add it to the mailbox wait queue
        // Treat errors here as fatal/panic
        if (queue_in(&mailboxes[mbox].wait_queue, current->pid) != 0) {
            panic("Unable to add process to mail box wait queue");
        }
        current->state = WAITING;
        scheduler_remove(current);
    
    }else{
        // if not empty, we need to queue a message out of the mailbox
        // set the message received time (in seconds)
        // Treat errors here at fatal/panic
        if (mbox_queue_out(mbox, msg) != 0) {
            panic("Unable to dequeue message from mailbox");
        }
        msg->time_received = system_time;
    }


    return 0;
}
