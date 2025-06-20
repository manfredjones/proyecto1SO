#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "canvas.h"
#include "monitor.h"
#include "parser.h"
#include "mypthread.h"
#include "scheduler.h"
#include "object_to_figure.h"

#define MONITOR_ROWS 2
#define MONITOR_COLS 2

extern Canvas canvas;
extern int global_time;

// Bucle principal de cada hilo monitor: actualiza y dibuja su región del canvas
void draw_full_canvas(Canvas *canvas, int current_time);  // Dibuja el canvas completo (solo el primer hilo lo hace)

void monitor_loop(void) {
    my_thread_t tid = current_thread_id;
    for (int t = 0; t <= 10; t++) {
        global_time = t;
        canvas_update(&canvas, t);  // Mueve figuras y detecta colisiones
        if (tid == 0) draw_full_canvas(&canvas, t);  // Solo el primer hilo dibuja la vista completa
        usleep(300000);  // Espera 300 ms entre frames
        my_thread_yield();  // Cede el turno al siguiente hilo
    }
    my_thread_end();  // Finaliza el hilo
}

int main(int argc, char *argv[]) {
    SchedulerType tipo = ROUND_ROBIN;

    // Permitir selección de scheduler desde la línea de comandos
    if (argc > 1) {
        if (strcmp(argv[1], "lottery") == 0) tipo = LOTTERY;
        else if (strcmp(argv[1], "realtime") == 0) tipo = REAL_TIME;
        else if (strcmp(argv[1], "rr") == 0) tipo = ROUND_ROBIN;
    }

    scheduler_init(tipo);

    // Cargar archivo de animación y configurar el canvas
    Animation anim;
    int object_count = parse_animation("build/animation.ani", &anim);
    canvas_init(&canvas, anim.width, anim.height);

    for (int i = 0; i < object_count; i++) {
        Figure *fig = object_to_figure(&anim.objects[i]);
        canvas_add_figure(&canvas, fig);
    }

    // Crear y distribuir monitores sobre el canvas
    int mon_width = canvas.width / MONITOR_COLS;
    int mon_height = canvas.height / MONITOR_ROWS;
    my_thread_t tids[MAX_MONITORS];
    int id = 0;

    for (int i = 0; i < MONITOR_ROWS; i++) {
        for (int j = 0; j < MONITOR_COLS; j++) {
            Monitor *mon = malloc(sizeof(Monitor));
            int x0 = j * mon_width;
            int y0 = i * mon_height;
            int x1 = x0 + mon_width - 1;
            int y1 = y0 + mon_height - 1;
            monitor_init(mon, x0, y0, x1, y1, &canvas);
            canvas_add_monitor(&canvas, mon);
            my_thread_create(&tids[id], monitor_loop, tipo);
            id++;
        }
    }

    // Inicia la ejecución de los hilos según el scheduler seleccionado
    scheduler_run();
    return 0;
}
