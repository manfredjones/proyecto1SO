#include "scheduler.h"
#include "mypthread.h"
#include <ucontext.h>
#include <stdio.h>

static Scheduler scheduler;

extern thread_control_block thread_table[MAX_THREADS];
extern int thread_count;
extern my_thread_t current_thread_id;

static int rr_index = 0;

void scheduler_init(SchedulerType type) {
    scheduler.type = type;
    rr_index = 0;
    current_thread_id = -1;
}

void scheduler_add(my_thread_t thread) {
    // TODO: Agregar un hilo a la cola del scheduler
}

my_thread_t scheduler_next() {
    // TODO: adaptar lógica de Lottery y Real-Time del folder reference_schedulers/
    int tries = 0;
    while (tries < thread_count) {
        rr_index = (rr_index + 1) % thread_count;
        if (thread_table[rr_index].state == READY) {
            return rr_index;
        }
        tries++;
    }
    return -1;
}

void scheduler_run() {
    if (thread_count == 0) return;

    current_thread_id = 0;
    thread_table[0].state = RUNNING;
    setcontext(&thread_table[0].context);
}