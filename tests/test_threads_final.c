
#include "../include/mypthread.h"
#include "../include/scheduler.h"
#include <stdio.h>

my_mutex_t mutex;
int recurso = 0;

void hilo_detach() {
    printf("Hilo detach inicializado\n");
    for (int i = 0; i < 2; i++) {
        my_mutex_lock(&mutex);
        recurso++;
        printf("Detach modifica recurso: %d\n", recurso);
        my_mutex_unlock(&mutex);
        my_thread_yield();
    }
    printf("Hilo detach terminando\n");
    my_thread_end();
}

void hilo_con_join() {
    my_thread_t t_detach;
    my_thread_create(&t_detach, hilo_detach, ROUND_ROBIN);
    my_thread_detach(t_detach);

    for (int i = 0; i < 3; i++) {
        my_mutex_lock(&mutex);
        recurso++;
        printf("Join modifica recurso: %d\n", recurso);
        my_mutex_unlock(&mutex);
        my_thread_yield();
    }

    my_thread_end();
}

int main() {
    scheduler_init(ROUND_ROBIN);
    my_mutex_init(&mutex);

    my_thread_t t_join;
    my_thread_create(&t_join, hilo_con_join, ROUND_ROBIN);

    scheduler_run();
    return 0;
}
