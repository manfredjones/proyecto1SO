#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUM_THREADS 3

typedef struct {
    pthread_t thread;
    int id;
    int deadline_ms; // Menor deadline = más prioridad
    int done;
} rt_thread_t;

rt_thread_t threads[NUM_THREADS];

// Simula obtener el tiempo actual en milisegundos
long long current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

void* thread_function(void* arg) {
    rt_thread_t* t = (rt_thread_t*)arg;
    printf(">> Hilo %d ejecutándose (deadline: %d ms)\n", t->id, t->deadline_ms);
    sleep(1); // simula trabajo
    printf("<< Hilo %d terminó\n", t->id);
    t->done = 1;
    return NULL;
}

int find_earliest_deadline(rt_thread_t threads[], int count) {
    int min = -1;
    for (int i = 0; i < count; i++) {
        if (!threads[i].done) {
            if (min == -1 || threads[i].deadline_ms < threads[min].deadline_ms)
                min = i;
        }
    }
    return min;
}

int main() {
    // Crear hilos con distintos deadlines
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].id = i;
        threads[i].done = 0;
        threads[i].deadline_ms = (i + 1) * 1000; // por ejemplo: 1000, 2000, 3000 ms
        pthread_create(&threads[i].thread, NULL, thread_function, &threads[i]);
    }

    // Scheduler tiempo real: EDF
    while (1) {
        int i = find_earliest_deadline(threads, NUM_THREADS);
        if (i == -1) break; // Todos terminaron

        pthread_join(threads[i].thread, NULL);
    }

    printf("Todos los hilos han terminado.\n");
    return 0;
}