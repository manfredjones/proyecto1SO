#include "canvas.h"

void canvas_init(Canvas *canvas, int width, int height) {
    canvas->width = width;
    canvas->height = height;
    canvas->figure_count = 0;
    canvas->monitor_count = 0;
}

void canvas_add_figure(Canvas *canvas, Figure *fig) {
    if (canvas->figure_count < MAX_FIGURES)
        canvas->figures[canvas->figure_count++] = fig;
}

void canvas_add_monitor(Canvas *canvas, Monitor *mon) {
    if (canvas->monitor_count < MAX_MONITORS)
        canvas->monitors[canvas->monitor_count++] = mon;
}

void canvas_update(Canvas *canvas, int current_time) {
    for (int i = 0; i < canvas->monitor_count; i++) {
        monitor_draw(canvas->monitors[i], current_time);
    }
}