
#include "../include/mypthread.h"
#include "../include/scheduler.h"
#include <stdio.h>

void tarea_secundaria() {
    for (int i = 0; i < 3; i++) {
        printf("Secundario: paso %d\n", i + 1);
        my_thread_yield();
    }
    printf("Secundario ha terminado correctamente\\n");
    my_thread_end();
}

void tarea_principal() {
    my_thread_t hijo;
    my_thread_create(&hijo, tarea_secundaria, ROUND_ROBIN);
    printf("Principal esperando a hijo\n");
    my_thread_join(hijo);
    printf("Principal continúa tras join\n");
    my_thread_end();
}

int main() {
    scheduler_init(ROUND_ROBIN);

    my_thread_t padre;
    my_thread_create(&padre, tarea_principal, ROUND_ROBIN);

    scheduler_run();

    return 0;
}
