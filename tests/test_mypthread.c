#include "mypthread.h"
#include "scheduler.h"
#include <stdio.h>

void tarea() {
    for (int i = 0; i < 5; i++) {
        printf("Hola desde hilo!\n");
        my_thread_yield();
    }
    my_thread_end();
}

int main() {
    scheduler_init(ROUND_ROBIN);

    my_thread_t t1;
    my_thread_create(&t1, tarea, ROUND_ROBIN);

    scheduler_run();  // función que ejecuta el ciclo de hilos

    return 0;
}
