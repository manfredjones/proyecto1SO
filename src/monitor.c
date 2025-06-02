#include "monitor.h"
#include "canvas.h"
#include "figure.h"
#include <stdio.h>


void monitor_init(Monitor *mon, int x0, int y0, int x1, int y1, Canvas *canvas) {
    printf("Canvas: %dx%d\n", canvas->width, canvas->height);
    printf("Monitor: (%d,%d)-(%d,%d)\n", x0, y0, x1, y1);
    mon->x0 = x0;
    mon->y0 = y0;
    mon->x1 = x1;
    mon->y1 = y1;
    mon->canvas = canvas;
    my_mutex_init(&mon->mutex);
}

void monitor_draw(Monitor *mon, int current_time) {
    my_mutex_lock(&mon->mutex);
    printf("Monitor (%d,%d) - (%d,%d) at time %d:\n", mon->x0, mon->y0, mon->x1, mon->y1, current_time);
    for (int i = 0; i < mon->canvas->figure_count; i++) {
        Figure *f = mon->canvas->figures[i];
        if (f->x >= mon->x0 && f->x <= mon->x1 && f->y >= mon->y0 && f->y <= mon->y1) {
            printf(" - Figura %s (%c) en (%d, %d)\n", f->label, f->symbol, f->x, f->y);
        }
    }
    my_mutex_unlock(&mon->mutex);
}

