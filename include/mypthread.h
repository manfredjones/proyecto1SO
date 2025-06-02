#ifndef MYPTHREAD_H
#define MYPTHREAD_H

#define STACK_SIZE 8192     // Tamaño de pila por hilo
#define MAX_THREADS 64      // Número máximo de hilos simultáneos

#include <ucontext.h>

// Identificador de hilo (simplemente un entero)
typedef int my_thread_t;

// Estructura para mutex simples basados en spinlock
typedef struct {
    int locked;             // 1 si está bloqueado, 0 si está libre
    my_thread_t owner;      // ID del hilo que posee el mutex
} my_mutex_t;

// Funciones de manejo de hilos


// Crea un nuevo hilo con una rutina y un tipo de scheduler
int my_thread_create(my_thread_t *thread, void (*start_routine)(void), int scheduler_type);

// Finaliza el hilo actual y transfiere control al siguiente
void my_thread_end();

// Cede voluntariamente el CPU al siguiente hilo
void my_thread_yield();

// Espera a que un hilo termine (bloqueante)
int my_thread_join(my_thread_t thread);

// Marca un hilo como detached (no puede ser esperado por join)
int my_thread_detach(my_thread_t thread);


// Funciones de manejo de mutex


// Inicializa un mutex
int my_mutex_init(my_mutex_t *mutex);

// Destruye un mutex
int my_mutex_destroy(my_mutex_t *mutex);

// Bloquea un mutex (espera activamente si está ocupado)
int my_mutex_lock(my_mutex_t *mutex);

// Libera un mutex (debe ser el dueño)
int my_mutex_unlock(my_mutex_t *mutex);

// Intenta adquirir un mutex sin bloquearse (pendiente de implementación)
int my_mutex_trylock(my_mutex_t *mutex);


// Función relacionada al scheduler


// Cambia el tipo de scheduler asociado a un hilo en tiempo de ejecución
int my_thread_chsched(my_thread_t thread, int scheduler_type);


// Estructuras internas del sistema de hilos


// Estados posibles de un hilo
typedef enum {
    READY,      // Listo para ejecutarse
    RUNNING,    // Actualmente ejecutándose
    FINISHED,   // Ya terminó
    BLOCKED     // Esperando a otro hilo (ej. join)
} thread_state;

// Bloque de control de cada hilo
typedef struct {
    int id;                         // ID del hilo
    ucontext_t context;             // Contexto de ejecución (ucontext)
    thread_state state;            // Estado actual del hilo
    void *retval;                  // Valor de retorno (no utilizado aún)
    int scheduler_type;           // Tipo de scheduler asignado
    char stack[STACK_SIZE];        // Pila de ejecución
    int waiting_thread_id;         // Hilo que espera a este (si aplica)
    int detached;                  // 1 si es detach, 0 si es joinable
} thread_control_block;

// Tabla global de hilos activos
extern thread_control_block thread_table[MAX_THREADS];
extern int thread_count;

// ID del hilo que está actualmente en ejecución
extern my_thread_t current_thread_id;

#endif // MYPTHREAD_H
