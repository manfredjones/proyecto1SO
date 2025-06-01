#include "../include/mypthread.h"
#include "../include/canvas.h"
#include "../include/monitor.h"
#include "../include/figure.h"
#include "../include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Monitor globalMonitor;
Canvas globalCanvas;

void monitor_loop() {
    for (int t = 0; t <= 5; t++) {
        monitor_draw(&globalMonitor, t);
        my_thread_yield(); // cede el control
        sleep(1); 
    }
    my_thread_end();
}

int main() {
    canvas_init(&globalCanvas, 20, 10);

    // Figura estática de prueba
    Figure *fig = malloc(sizeof(Figure));
    fig->x = 5;
    fig->y = 5;
    fig->symbol = 'o';
    snprintf(fig->label, sizeof(fig->label), "Pelota");

    canvas_add_figure(&globalCanvas, fig);

    // Monitor que cubre todo el canvas
    monitor_init(&globalMonitor, 0, 0, 19, 9, &globalCanvas);
    canvas_add_monitor(&globalCanvas, &globalMonitor);

    my_thread_t tid;
    my_thread_create(&tid, monitor_loop, ROUND_ROBIN);

    scheduler_run();  // comienza ejecución de hilos

    return 0;
}