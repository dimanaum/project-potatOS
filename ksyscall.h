#ifndef KSYSCALL_H
#define KSYSCALL_H

#include "syscall_defs.h"
#include "msg.h"

/* System Call Handler */
int ksyscall_handler(int syscall, unsigned int arg1, unsigned int arg2, unsigned int arg3);

/* System information */
int ksyscall_sys_get_time(void);

/* Process functionality */
int ksyscall_yield(void);
int ksyscall_sleep(int time);

int ksyscall_proc_exec(char *proc_name, void *proc_ptr);
int ksyscall_proc_exit(void);

/* Process information */
int ksyscall_proc_get_pid(void);
int ksyscall_proc_get_name(char *name);

/* Mutex functions */
int ksyscall_mutex_alloc(void);
int ksyscall_mutex_free(int id);
int ksyscall_mutex_lock(int id);
int ksyscall_mutex_unlock(int id);

/* Message functions */
int ksyscall_msg_send(int mbox, msg_t *msg);
int ksyscall_msg_recv(int mbox, msg_t *msg);

#endif
