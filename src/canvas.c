#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "canvas.h"
#include "monitor.h"
#include "figure.h"

int global_time = 0;
Canvas canvas;

void canvas_init(Canvas* c, int width, int height) {
    c->width = width;
    c->height = height;
    c->figure_count = 0;
    c->monitor_count = 0;
}

void canvas_add_figure(Canvas* c, Figure* f) {
    if (c->figure_count < MAX_FIGURES) {
        c->figures[c->figure_count++] = f;
    }
}

void canvas_add_monitor(Canvas* c, Monitor* m) {
    if (c->monitor_count < MAX_MONITORS) {
        c->monitors[c->monitor_count++] = m;
    }
}

void canvas_update(Canvas* c, int time) {
    for (int i = 0; i < c->figure_count; i++) {
        Figure* fig = c->figures[i];
        if (time >= fig->start_time && time <= fig->end_time) {
            figure_move(fig);
        }
    }
}

void canvas_draw(Canvas* c, int time, int monitor_id) {
    Monitor* m = c->monitors[monitor_id];
    printf("Monitor (%d,%d) - (%d,%d) at time %d:\n", m->x0, m->y0, m->x1, m->y1, time);
    for (int i = 0; i < c->figure_count; i++) {
        Figure* fig = c->figures[i];
        if (time >= fig->start_time && time <= fig->end_time) {
            if (fig->x >= m->x0 && fig->x <= m->x1 &&
                fig->y >= m->y0 && fig->y <= m->y1) {
                printf(" - Figura %s (%c) en (%d, %d)\n", fig->label, fig->symbol, fig->x, fig->y);
            }
        }
    }
}
