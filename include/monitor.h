
#ifndef MONITOR_H
#define MONITOR_H

#include "canvas.h"

typedef struct {
    int id;
    int x_start, x_end;
    int y_start, y_end;
    Canvas *canvas;
} Monitor;

void monitor_init(Monitor *m, int id, int x0, int x1, int y0, int y1, Canvas *canvas);
void monitor_render(Monitor *m);

#endif // MONITOR_H
