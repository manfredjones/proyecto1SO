
#include "../include/monitor.h"
#include <stdio.h>

void monitor_init(Monitor *m, int id, int x0, int x1, int y0, int y1, Canvas *canvas) {
    m->id = id;
    m->x_start = x0;
    m->x_end = x1;
    m->y_start = y0;
    m->y_end = y1;
    m->canvas = canvas;
}

void monitor_render(Monitor *m) {
    printf("Monitor %d rendering from (%d,%d) to (%d,%d)\n",
           m->id, m->x_start, m->y_start, m->x_end, m->y_end);
}
