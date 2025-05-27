#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>

#define NUM_THREADS 3
#define WORK_PER_THREAD 3

typedef struct {
    int id;
    int tickets;
    pthread_mutex_t mutex;
} ThreadInfo;

ThreadInfo threads[NUM_THREADS];
pthread_t thread_ids[NUM_THREADS];

void* thread_function(void* arg) {
    ThreadInfo* info = (ThreadInfo*)arg;

    for (int i = 0; i < WORK_PER_THREAD; i++) {
        pthread_mutex_lock(&info->mutex);  // espera su turno

        printf("Hilo %d (tickets: %d) trabajando paso %d\n", info->id, info->tickets, i + 1);
        usleep(100000);  // simula trabajo

        pthread_mutex_unlock(&info->mutex);  // cede control
        sched_yield();
    }

    pthread_exit(NULL);
}

int total_tickets() {
    int total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total += threads[i].tickets;
    }
    return total;
}

int lottery_draw() {
    int total = total_tickets();
    int r = rand() % total;
    int sum = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        sum += threads[i].tickets;
        if (r < sum) return i;
    }
    return 0;  // fallback
}

int main() {
    srand(time(NULL));

    // inicializar mutex y hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].id = i;
        threads[i].tickets = (i + 1) * 2;  // diferentes tickets
        pthread_mutex_init(&threads[i].mutex, NULL);
        pthread_mutex_lock(&threads[i].mutex);  // los hilos esperan su turno

        pthread_create(&thread_ids[i], NULL, thread_function, &threads[i]);
    }

    // scheduler principal (turno por sorteo)
    int total_steps = NUM_THREADS * WORK_PER_THREAD;
    for (int step = 0; step < total_steps; step++) {
        int winner = lottery_draw();
        pthread_mutex_unlock(&threads[winner].mutex);  // permitir que trabaje

        usleep(150000);  // da tiempo para ejecutar
    }

    // esperar a que terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_ids[i], NULL);
        pthread_mutex_destroy(&threads[i].mutex);
    }

    return 0;
}