#ifndef CANVAS_H
#define CANVAS_H

#define MAX_FIGURES 100
#define MAX_MONITORS 10

#include "figure.h"
#include "monitor.h"

typedef struct Canvas {
    int width, height;
    Figure *figures[MAX_FIGURES];
    int figure_count;
    struct Monitor *monitors[MAX_MONITORS]; 
    int monitor_count;
} Canvas;

void canvas_init(Canvas *canvas, int width, int height);
void canvas_add_figure(Canvas *canvas, Figure *fig);
void canvas_add_monitor(Canvas *canvas, struct Monitor *mon);
void canvas_update(Canvas *canvas, int current_time);


#endif

