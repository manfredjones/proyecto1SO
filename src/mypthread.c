
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
    thread_table[current_thread_id].state = FINISHED;

    int next_id = scheduler_next();
    if (next_id == -1) {
        // No hay más hilos, salir del programa
        exit(0);
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

int my_thread_join(my_thread_t thread) {
    // TODO: Esperar a que un hilo termine
    return 0;
}

int my_thread_detach(my_thread_t thread) {
    // TODO: Marcar un hilo como detach
    return 0;
}

int my_mutex_init(my_mutex_t *mutex) {
    // TODO: Inicializar el mutex
    return 0;
}

int my_mutex_destroy(my_mutex_t *mutex) {
    // TODO: Destruir el mutex
    return 0;
}

int my_mutex_lock(my_mutex_t *mutex) {
    // TODO: Bloquear el mutex
    return 0;
}

int my_mutex_unlock(my_mutex_t *mutex) {
    // TODO: Desbloquear el mutex
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