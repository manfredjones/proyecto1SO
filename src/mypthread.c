
#include "mypthread.h"
#include "scheduler.h"
#include <ucontext.h>
#include <stdlib.h>
#include <string.h>

#define MAX_THREADS 64
#define STACK_SIZE 8192

extern my_thread_t current_thread_id;

// Tabla global de hilos
thread_control_block thread_table[MAX_THREADS];
int thread_count = 0;

int my_thread_create(my_thread_t *thread, void (*start_routine)(void), int scheduler_type) {
    if (thread_count >= MAX_THREADS)
        return -1;

    int id = thread_count;
    thread_control_block *tcb = &thread_table[id];

    // Crear un nuevo contexto
    tcb->waiting_thread_id = -1;
    getcontext(&tcb->context);
    tcb->context.uc_stack.ss_sp = tcb->stack;
    tcb->context.uc_stack.ss_size = STACK_SIZE;
    tcb->context.uc_link = NULL;  // cuando termine, se finaliza el hilo
    makecontext(&tcb->context, start_routine, 0);

    // Inicializar datos del hilo
    tcb->id = id;
    tcb->state = READY;
    tcb->retval = NULL;
    tcb->scheduler_type = scheduler_type;

    *thread = id;
    thread_count++;

    // Registrar el hilo en el scheduler correspondiente
    scheduler_add(id);  // aquí debes hacer que el scheduler sepa a qué tipo pertenece

    return 0;
}

void my_thread_end() {
    thread_control_block *current = &thread_table[current_thread_id];
    current->state = FINISHED;

    // Si alguien está esperando a este hilo, desbloquearlo
    if (current->waiting_thread_id != -1) {
        thread_table[current->waiting_thread_id].state = READY;
        current->waiting_thread_id = -1;
    }


    int next_id = scheduler_next();
    if (next_id == -1) {
        exit(0);  // No hay más hilos listos
    }

    thread_table[next_id].state = RUNNING;
    current_thread_id = next_id;
    setcontext(&thread_table[next_id].context);
}
my_thread_t current_thread_id = -1;

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

int my_thread_join(my_thread_t thread_id) {
    // 1. Verificamos que el ID sea válido
    if (thread_id < 0 || thread_id >= thread_count)
        return -1;

    thread_control_block *target = &thread_table[thread_id];

    // 2. Si ya terminó, no hay que esperar
    if (target->state == FINISHED)
        return 0;

    // 3. Guardamos el hilo que espera en el hilo objetivo
    target->waiting_thread_id = current_thread_id;

    // 4. Marcamos al hilo actual como bloqueado
    thread_table[current_thread_id].state = BLOCKED;

    // 5. Cedemos el control a otro hilo
    while (target->state != FINISHED)
    my_thread_yield();

    return 0;
}



int my_thread_detach(my_thread_t thread) {
    // TODO: Marcar un hilo como detach
    return 0;
}

int my_mutex_init(my_mutex_t *mutex) {
    if (!mutex) return -1;
    mutex->locked = 0;
    mutex->owner = -1;
    return 0;
}

int my_mutex_destroy(my_mutex_t *mutex) {
    if (!mutex) return -1;
    mutex->locked = 0;
    mutex->owner = -1;
    return 0;
}

int my_mutex_lock(my_mutex_t *mutex) {
    if (!mutex) return -1;

    while (__sync_lock_test_and_set(&mutex->locked, 1)) {
        // Ya está bloqueado, ceder turno (cooperativamente)
        my_thread_yield();
    }

    // Éxito al adquirirlo
    mutex->owner = current_thread_id;
    return 0;
}

int my_mutex_unlock(my_mutex_t *mutex) {
    if (!mutex || mutex->owner != current_thread_id) return -1;

    mutex->owner = -1;
    __sync_lock_release(&mutex->locked);
    return 0;
}

int my_mutex_trylock(my_mutex_t *mutex) {
    // TODO: Intentar bloquear el mutex sin bloquearse
    return 0;
}

int my_thread_chsched(my_thread_t thread, int scheduler_type) {
    // TODO: Cambiar el scheduler de un hilo
    return 0;
}