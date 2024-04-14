/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2021
 *
 * User/Test programs
 */
#include <spede/flames.h>
#include <spede/stdio.h>
#include <spede/string.h>

#include "prog.h"
#include "syscall.h"
#include "util.h"
#include "spin_lock.h"

#define BUF_LEN 256
#define WORD_COUNT 9

struct test_data {
    int sequence;
    char word[24];
};

char *words[WORD_COUNT] = {"the", "quick", "brown", "fox",
                           "jumps", "over", "the", "lazy", "dog"};

// Mutex for "odd" processes
int odd_mutex;

// Mutex for "even" processes
int even_mutex;

// Lock for "odd" processes
int odd_lock;

// Lock for "even" processe
int even_lock;


/**
 * Test "program" that will just "sleep" over and over forever
 */
void prog_forever_sleep() {
    // Sleep for 10s forever and ever
    while(1) {
        sleep(10);
    }
}

/**
 * Test "program" that will just "delay" over and over forever
 */
void prog_forever_delay() {
    // Delay for 10s forever and ever
    while(1) {
        delay(10);
    }
}

/**
 * User program to initialize/execute other programs at startup
 * Will never return/exit. Once programs are executed, will loop forever
 * and yield the CPU.
 */
void prog_init() {
    int pid;
    int prog_pid;
    int time;
    char name[BUF_LEN];
    int i;

    struct init_programs {
        char *name;
        void (*code)(void*);
    };

    struct init_programs init_programs[] = {
        {
            .name = "sleep forever",
            .code = &prog_forever_sleep
        },
        {
            .name = "delay forever",
            .code = &prog_forever_delay
        }
    };

    sleep(5);

    pid = proc_get_pid();
    proc_get_name(name);

    // Allocate mutexes
    odd_mutex = mutex_alloc();
    even_mutex = mutex_alloc();

    // Initialize locks
    odd_lock = 0;
    even_lock = 0;

    if (odd_mutex == -1 || even_mutex == -1) {
        printf("Invalid mutexes!");
        proc_exit();
    }

    for (i = 0; i < (sizeof(init_programs)/sizeof(struct init_programs)); i++) {
        time = sys_get_time();
        prog_pid = proc_exec(init_programs[i].code, init_programs[i].name);
        printf("%04d (pid=%d, name='%s') executed process (pid=%d, name='%s')\n", time, pid, name, prog_pid, init_programs[i].name);
    }

    while(1) {
        /* Nothing to do at this point so just yield the CPU */
        yield();
    }
}

/**
 * Test program to exercise various system calls
 *
 * Iterates over alternating sleep or busy delays and then finally exits.
 * Exercises a variety of system calls to make sure they are functional
 */
void prog_test() {
    int pid;                // To store the process id
    char name[BUF_LEN];     // To store the process name

    int sleep_time;         // How long to sleep/delay
    int current_time;       // The current system time (in seconds)
    int count;              // How many iterations to take

    int *mutex;             // Which mutex?
    int *lock;              // Which lock?

    int lock_type;          // mutex or spinlock?

    // Get the PID for this process
    pid = proc_get_pid();

    // Get the name of this process
    proc_get_name(name);

    // Get the current system time
    current_time = sys_get_time();

    // Calculate a sleep/delay time and iteration count for the program
    sleep_time = (pid % 5);
    count = (sleep_time + 1) * 3;

    if (pid % 2 == 0) {
        mutex = &even_mutex;
        lock = &even_lock;
    } else {
        mutex = &odd_mutex;
        lock = &odd_lock;
    }

    cons_printf("%04d (pid=%d, name='%s') starting loop for %d iterations\n", current_time, pid, name, count);

    if (current_time % 2) {
        lock_type = 0;
    } else {
        lock_type = 1;
    }

//    mutex = &odd_mutex;
//    lock_type = 0;

    // Loop for a specified number of times
    while (count-- >= 0) {
        current_time=sys_get_time();

        if (lock_type == 0) {
            cons_printf("%04d (pid=%d, name='%s') mutex lock\n", current_time, pid, name);
            mutex_lock(*mutex);
        } else {
            cons_printf("%04d (pid=%d, name='%s') spin lock\n", current_time, pid, name);
            spin_lock(lock);
        }

        current_time=sys_get_time();
        cons_printf("%04d (pid=%d, name='%s') entered critical section\n", current_time, pid, name);

        // Either delay or sleep for some period of time
        if (current_time % 2 == 0) {
//            cons_printf("%04d (pid=%d, name='%s', count=%d) sleeping for %ds\n", current_time, pid, name, count, sleep_time);
            sleep(sleep_time);
        } else {
//            cons_printf("%04d (pid=%d, name='%s', count=%d) delaying for %ds\n", current_time, pid, name, count, sleep_time);
            delay(sleep_time);
        }

        current_time=sys_get_time();
        if (lock_type == 0) {
            cons_printf("%04d (pid=%d, name='%s') mutex unlock\n", current_time, pid, name);
            mutex_unlock(*mutex);
        } else {
            cons_printf("%04d (pid=%d, name='%s') spin unlock\n", current_time, pid, name);
            spin_unlock(lock);
        }
    }

    // Get the current system time and exit
    current_time=sys_get_time();
    cons_printf("%04d (pid=%d, name='%s') exiting\n", current_time, pid, name);

    proc_exit();
}


void prog_producer() {
    msg_t msg;
    struct test_data test_data;

    int pid = proc_get_pid();
    int mbox = pid % 2;
    int current_time = sys_get_time();
    int start_time = sys_get_time();

    memset(&test_data, 0, sizeof(struct test_data));

    while (current_time - start_time <= ((pid * 2) % 15) && test_data.sequence != WORD_COUNT) {
        current_time = sys_get_time();

        strcpy((char *)&test_data.word, (char *)words[test_data.sequence % WORD_COUNT]);
        memcpy(&msg.data, (unsigned char *)&test_data, sizeof(struct test_data));

        test_data.sequence++;

        if (msg_send(mbox, &msg) != 0) {
            cons_printf("pid=%d: Unable to send message... exiting\n", pid);
            proc_exit();
        }

        sleep(1);
    }

    proc_exit();
}

void prog_consumer() {
    msg_t msg;
    struct test_data test_data;

    int pid = proc_get_pid();
    int mbox = pid % 2;
    int current_time = sys_get_time();
    int start_time = sys_get_time();

    memset(&test_data, 0, sizeof(struct test_data));

    while (current_time - start_time <= ((pid * 2) % 15)) {
        if (msg_recv(mbox, &msg) != 0) {
            cons_printf("pid=%d: Unable to receive message... exiting\n", pid);
            proc_exit();
        }

        current_time = sys_get_time();

        memcpy(&test_data, (struct test_data *)&msg.data, sizeof(struct test_data));

        cons_printf("%04d (pid=%d) Received msg (sender=%d, sent=%d, recv=%d)\n",
                    current_time, pid, msg.sender, msg.time_sent, msg.time_received);

        cons_printf("\t\tdata (sequence=%d, word='%s')\n",
                    test_data.sequence, test_data.word);
    }

    proc_exit();
}

