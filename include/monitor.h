#ifndef MONITOR_H
#define MONITOR_H

#include "mypthread.h"

struct Canvas; 

typedef struct Monitor {
    int x0, y0, x1, y1;
    struct Canvas *canvas;
    my_mutex_t mutex;
} Monitor;

void monitor_init(Monitor *mon, int x0, int y0, int x1, int y1, struct Canvas *canvas);
void monitor_draw(Monitor *mon, int current_time);

#endif
