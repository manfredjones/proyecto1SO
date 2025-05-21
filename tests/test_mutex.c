
#include "../include/mypthread.h"
#include "../include/scheduler.h"
#include <stdio.h>

#define INCREMENTS 5

extern my_thread_t current_thread_id;


int contador = 0;
my_mutex_t mutex;

void hilo_incrementador() {
    for (int i = 0; i < INCREMENTS; i++) {
        my_mutex_lock(&mutex);
        int temp = contador;
        printf("Hilo %d incrementando: %d -> %d\n", current_thread_id, temp, temp + 1);
        contador = temp + 1;
        my_mutex_unlock(&mutex);
        my_thread_yield();
    }
    my_thread_end();
}

int main() {
    scheduler_init(ROUND_ROBIN);
    my_mutex_init(&mutex);

    my_thread_t t1, t2;
    my_thread_create(&t1, hilo_incrementador, ROUND_ROBIN);
    my_thread_create(&t2, hilo_incrementador, ROUND_ROBIN);

    scheduler_run();

    return 0;
}
