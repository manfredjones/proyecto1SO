#include "mypthread.h"
#include "scheduler.h"
#include <ucontext.h>
#include <stdlib.h>
#include <string.h>

#define MAX_THREADS 64
#define STACK_SIZE 8192

// ID del hilo que está actualmente en ejecución
my_thread_t current_thread_id = -1;

// Tabla de control de hilos
thread_control_block thread_table[MAX_THREADS];
int thread_count = 0;

// Crea un nuevo hilo con el scheduler especificado
int my_thread_create(my_thread_t *thread, void (*start_routine)(void), int scheduler_type) {
    if (thread_count >= MAX_THREADS)
        return -1;

    int id = thread_count;
    thread_control_block *tcb = &thread_table[id];

    // Inicializar contexto del hilo
    tcb->waiting_thread_id = -1;
    getcontext(&tcb->context);
    tcb->context.uc_stack.ss_sp = tcb->stack;
    tcb->context.uc_stack.ss_size = STACK_SIZE;
    tcb->context.uc_link = NULL;  // Al terminar, el hilo finaliza
    makecontext(&tcb->context, start_routine, 0);

    // Inicializar metadatos del hilo
    tcb->id = id;
    tcb->state = READY;
    tcb->retval = NULL;
    tcb->scheduler_type = scheduler_type;
    tcb->detached = 0;

    *thread = id;
    thread_count++;

    // Registrar el hilo en el scheduler
    scheduler_add(id);

    return 0;
}

// Termina el hilo actual y transfiere el control al siguiente
void my_thread_end() {
    thread_control_block *current = &thread_table[current_thread_id];
    current->state = FINISHED;

    if (current->detached) {
        // Evitar que otros hilos esperen este hilo
        current->waiting_thread_id = -1;
    }

    // Desbloquear al hilo que estaba esperando (si aplica)
    if (current->waiting_thread_id != -1) {
        thread_table[current->waiting_thread_id].state = READY;
        current->waiting_thread_id = -1;
    }

    // Seleccionar el siguiente hilo listo
    int next_id = scheduler_next();
    if (next_id == -1) {
        exit(0);  // No quedan hilos
    }

    thread_table[next_id].state = RUNNING;
    current_thread_id = next_id;
    setcontext(&thread_table[next_id].context);
}

// Cede el control a otro hilo según el scheduler
void my_thread_yield() {
    int next_id = scheduler_next();
    if (next_id == current_thread_id || next_id == -1)
        return;

    thread_control_block *current = &thread_table[current_thread_id];
    thread_control_block *next = &thread_table[next_id];

    current->state = READY;
    next->state = RUNNING;

    my_thread_t prev_id = current_thread_id;
    current_thread_id = next_id;

    swapcontext(&current->context, &next->context);
}

// Espera a que un hilo termine su ejecución
int my_thread_join(my_thread_t thread_id) {
    if (thread_id < 0 || thread_id >= thread_count)
        return -1;

    thread_control_block *target = &thread_table[thread_id];

    if (target->state == FINISHED)
        return 0;

    target->waiting_thread_id = current_thread_id;
    thread_table[current_thread_id].state = BLOCKED;

    while (target->state != FINISHED)
        my_thread_yield();

    return 0;
}

// Marca un hilo como detach (no será esperado por nadie)
int my_thread_detach(my_thread_t thread_id) {
    if (thread_id < 0 || thread_id >= thread_count)
        return -1;

    thread_control_block *tcb = &thread_table[thread_id];
    tcb->detached = 1;
    return 0;
}

// Inicializa un mutex
int my_mutex_init(my_mutex_t *mutex) {
    if (!mutex) return -1;
    mutex->locked = 0;
    mutex->owner = -1;
    return 0;
}

// Destruye un mutex
int my_mutex_destroy(my_mutex_t *mutex) {
    if (!mutex) return -1;
    mutex->locked = 0;
    mutex->owner = -1;
    return 0;
}

// Intenta adquirir un mutex bloqueando si está ocupado
int my_mutex_lock(my_mutex_t *mutex) {
    if (!mutex) return -1;

    while (__sync_lock_test_and_set(&mutex->locked, 1)) {
        my_thread_yield();  // Espera cooperativa
    }

    mutex->owner = current_thread_id;
    return 0;
}

// Libera un mutex si el hilo actual es el dueño
int my_mutex_unlock(my_mutex_t *mutex) {
    if (!mutex || mutex->owner != current_thread_id) return -1;

    mutex->owner = -1;
    __sync_lock_release(&mutex->locked);
    return 0;
}

// Intenta adquirir un mutex sin bloquear (incompleto)
int my_mutex_trylock(my_mutex_t *mutex) {
    // Implementación pendiente
    return 0;
}

// Cambia el tipo de scheduler asociado a un hilo
int my_thread_chsched(my_thread_t thread, int scheduler_type) {
    if (thread < 0 || thread >= thread_count)
        return -1;

    thread_table[thread].scheduler_type = scheduler_type;
    return 0;
}
