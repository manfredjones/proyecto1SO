#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "mypthread.h"

typedef enum {
    ROUND_ROBIN,
    LOTTERY,
    REAL_TIME
} SchedulerType;

typedef struct {
    SchedulerType type;
    my_thread_t *queue;
    int count;
} Scheduler;

void scheduler_init(SchedulerType type);
void scheduler_add(my_thread_t thread);
my_thread_t scheduler_next();
void scheduler_run();

#endif // SCHEDULER_H