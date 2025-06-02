#include "scheduler.h"
#include "mypthread.h"
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h> 

static Scheduler scheduler;

extern thread_control_block thread_table[MAX_THREADS];
extern int thread_count;
extern my_thread_t current_thread_id;

static int rr_index = 0;  // Índice del último hilo ejecutado en Round Robin

// Inicializa el scheduler con el tipo especificado
void scheduler_init(SchedulerType type) {
    scheduler.type = type;
    rr_index = 0;
    current_thread_id = -1;
}

// (Placeholder) Podría utilizarse para añadir metadatos de hilos al scheduler
void scheduler_add(my_thread_t thread) {
    // No se requiere implementación explícita actualmente.
}

// Selecciona el próximo hilo a ejecutar según el tipo de scheduler
my_thread_t scheduler_next() {
    if (scheduler.type == ROUND_ROBIN) {
        // Avanza circularmente hasta encontrar un hilo READY con tipo Round Robin
        int tries = 0;
        while (tries < thread_count) {
            rr_index = (rr_index + 1) % thread_count;
            if (thread_table[rr_index].state == READY &&
                thread_table[rr_index].scheduler_type == ROUND_ROBIN) {
                return rr_index;
            }
            tries++;
        }
        return -1;

    } else if (scheduler.type == LOTTERY) {
        // Asigna más "boletos" a hilos de menor índice (o prioridad personalizada)
        int total_tickets = 0;
        int tickets[MAX_THREADS];

        for (int i = 0; i < thread_count; i++) {
            if (thread_table[i].state == READY &&
                thread_table[i].scheduler_type == LOTTERY) {
                tickets[i] = (i + 1);  // Aquí podrías usar un campo de prioridad
                total_tickets += tickets[i];
            } else {
                tickets[i] = 0;
            }
        }

        if (total_tickets == 0) return -1;

        // Sorteo aleatorio basado en tickets
        int r = rand() % total_tickets;
        int sum = 0;
        for (int i = 0; i < thread_count; i++) {
            sum += tickets[i];
            if (r < sum) return i;
        }

        return -1;

    } else if (scheduler.type == REAL_TIME) {
        // Selecciona el hilo READY con menor ID (más prioritario)
        int best = -1;
        for (int i = 0; i < thread_count; i++) {
            if (thread_table[i].state == READY &&
                thread_table[i].scheduler_type == REAL_TIME) {
                if (best == -1 || i < best) {
                    best = i;
                }
            }
        }
        return best;
    }

    return -1;
}

// Ejecuta todos los hilos hasta que no quede ninguno en estado READY
void scheduler_run() {
    while (1) {
        int next_id = scheduler_next();
        if (next_id == -1) {
            break;  // No hay más hilos listos
        }

        current_thread_id = next_id;
        thread_table[next_id].state = RUNNING;
        setcontext(&thread_table[next_id].context);
    }

    // Todos los hilos han terminado su ejecución
}
