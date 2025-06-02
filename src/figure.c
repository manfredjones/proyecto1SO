#include "../include/figure.h"

void figure_move(Figure *fig) {
    extern int global_time;
    if (global_time < fig->start_time || global_time > fig->end_time)
        return;

    if (fig->move_type == FIG_MOVE_BOUNCE) {
        fig->x += fig->dx;
        fig->y += fig->dy;

        if (fig->x < fig->x1 || fig->x > fig->x2) {
            fig->dx *= -1;
            fig->x += fig->dx;
        }
        if (fig->y < fig->y1 || fig->y > fig->y2) {
            fig->dy *= -1;
            fig->y += fig->dy;
        }
    } else if (fig->move_type == FIG_MOVE_LINEAR) {
        float total_time = fig->duration_ms / 100.0f; // cada frame es 100ms
        float elapsed = (float)(global_time - fig->start_time);

        if (elapsed > total_time) elapsed = total_time;
        if (elapsed < 0) elapsed = 0;

        float t = elapsed / total_time;

        fig->x = fig->x_start + (int)((fig->x_end - fig->x_start) * t);
        fig->y = fig->y_start + (int)((fig->y_end - fig->y_start) * t);
    }
    // FIG_MOVE_NONE: no hace nada
}

int figure_collides(const Figure *a, const Figure *b) {
    return (a->x == b->x) && (a->y == b->y);
}
