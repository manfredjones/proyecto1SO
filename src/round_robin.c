#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#define NUM_THREADS 3
#define ROUNDS 5

pthread_mutex_t lock;
pthread_cond_t cond;
int current_thread = 0; // ID del hilo que puede ejecutar
int turn = 0;           // Turno global compartido

void my_thread_yield() {
    sched_yield(); // o pthread_yield_np() en Mac
}

void* thread_function(void* arg) {
    int id = *(int*)arg;

    for (int r = 0; r < ROUNDS; r++) {
        pthread_mutex_lock(&lock);

        while (turn % NUM_THREADS != id) {
            pthread_cond_wait(&cond, &lock);
        }

        // Sección crítica (turno del hilo)
        printf("Hilo %d está ejecutando su ronda %d\n", id, r + 1);

        // Simula trabajo
        usleep(100000); // 0.1 segundos

        turn++; // Avanza el turno
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);

        my_thread_yield();
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    printf("Todos los hilos han terminado.\n");
    return 0;
}