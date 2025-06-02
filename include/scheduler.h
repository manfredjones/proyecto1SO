#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "mypthread.h"

// Tipos de scheduler disponibles
typedef enum {
    ROUND_ROBIN,
    LOTTERY,
    REAL_TIME
} SchedulerType;

// Estructura del scheduler
typedef struct {
    SchedulerType type;
    my_thread_t *queue;
    int count;
} Scheduler;

void scheduler_init(SchedulerType type);    // Inicializa el scheduler
void scheduler_add(my_thread_t thread);     // Registra un hilo
my_thread_t scheduler_next();               // Selecciona el próximo hilo
void scheduler_run();                       // Ejecuta hilos hasta que terminen

#endif // SCHEDULER_H
