#include "scheduler.h"
#include "mypthread.h"
#include <ucontext.h>
#include <stdio.h>

static Scheduler scheduler;

extern thread_control_block thread_table[MAX_THREADS];
extern int thread_count;

void scheduler_init(SchedulerType type) {
    // TODO: Inicializar el scheduler con el tipo dado
    scheduler.type = type;
}

void scheduler_add(my_thread_t thread) {
    // TODO: Agregar un hilo a la cola del scheduler
}

my_thread_t scheduler_next() {
    // TODO: Obtener el siguiente hilo a ejecutar
    return -1;
}

void scheduler_run() {
    for (int i = 0; i < thread_count; i++) {
        setcontext(&thread_table[i].context);
    }
}