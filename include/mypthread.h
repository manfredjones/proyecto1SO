#ifndef MYPTHREAD_H
#define MYPTHREAD_H
#define STACK_SIZE 8192
#define MAX_THREADS 64


#include <ucontext.h>

typedef int my_thread_t;

typedef struct {
    int locked;
    my_thread_t owner;
} my_mutex_t;

// Thread control functions
int my_thread_create(my_thread_t *thread, void (*start_routine)(void), int scheduler_type);
void my_thread_end();
void my_thread_yield();
int my_thread_join(my_thread_t thread);
int my_thread_detach(my_thread_t thread);


// Mutex functions
int my_mutex_init(my_mutex_t *mutex);
int my_mutex_destroy(my_mutex_t *mutex);
int my_mutex_lock(my_mutex_t *mutex);
int my_mutex_unlock(my_mutex_t *mutex);
int my_mutex_trylock(my_mutex_t *mutex);

// Scheduler-related
int my_thread_chsched(my_thread_t thread, int scheduler_type);

typedef enum {
    READY,
    RUNNING,
    FINISHED,
    BLOCKED
} thread_state;

typedef struct {
    int id;
    ucontext_t context;
    thread_state state;
    void *retval;
    int scheduler_type;
    char stack[STACK_SIZE];
    int waiting_thread_id;  // -1 si nadie lo espera, si no, guarda el ID del que hizo join
} thread_control_block;

extern thread_control_block thread_table[MAX_THREADS];
extern int thread_count;

#endif // MYPTHREAD_H