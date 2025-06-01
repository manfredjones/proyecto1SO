#include "../include/mypthread.h"
#include "../include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Variables externas del sistema de hilos
extern my_thread_t current_thread_id;
extern thread_control_block thread_table[MAX_THREADS];
extern int thread_count;

void work_function() {
    my_thread_t tid = current_thread_id;
    for (int i = 0; i < 5; i++) {
        printf("Hilo %d ejecutando iteración %d\n", tid, i);
        usleep(100 * 1000); // Simula trabajo
        my_thread_yield();
    }
    my_thread_end();
}

void reset_threads() {
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_table[i].state = FINISHED;
        thread_table[i].waiting_thread_id = -1;
        thread_table[i].detached = 0;
    }
    thread_count = 0;
}

// Ejecuta 5 hilos bajo un tipo de scheduler
void run_test(SchedulerType schedType, const char* name) {
    printf("\n========== Probando %s ==========\n", name);
    scheduler_init(schedType);

    for (int i = 0; i < 5; i++) {
        my_thread_t tid;
        if (my_thread_create(&tid, work_function, schedType) != 0) {
            fprintf(stderr, "Error creando hilo %d\n", i);
        }
    }

    scheduler_run();
}

int main() {
    if (fork() == 0) {
        run_test(ROUND_ROBIN, "Round Robin");
        exit(0);
    }
    wait(NULL);

    if (fork() == 0) {
        run_test(LOTTERY, "Lottery Scheduler");
        exit(0);
    }
    wait(NULL);

    if (fork() == 0) {
        run_test(REAL_TIME, "Real-Time Scheduler");
        exit(0);
    }
    wait(NULL);

    return 0;
}

