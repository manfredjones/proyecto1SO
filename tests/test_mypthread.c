#include "../include/mypthread.h"
#include "../include/scheduler.h"
#include <stdio.h>

void tareaA() {
    for (int i = 0; i < 3; i++) {
        printf("A\n");
        my_thread_yield();
    }
    my_thread_end();
}

void tareaB() {
    for (int i = 0; i < 3; i++) {
        printf("B\n");
        my_thread_yield();
    }
    my_thread_end();
}

int main() {
    scheduler_init(ROUND_ROBIN);

    my_thread_t t1, t2;
    my_thread_create(&t1, tareaA, ROUND_ROBIN);
    my_thread_create(&t2, tareaB, ROUND_ROBIN);

    scheduler_run();

    return 0;
}
