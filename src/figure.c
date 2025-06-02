#include "../include/figure.h"

// Actualiza la posición de una figura según su tipo de movimiento y el tiempo global
void figure_move(Figure *fig) {
    extern int global_time;

    // No hacer nada si la figura aún no ha iniciado o ya terminó su tiempo
    if (global_time < fig->start_time || global_time > fig->end_time)
        return;

    if (fig->move_type == FIG_MOVE_BOUNCE) {
        // Movimiento con rebote dentro de un área delimitada
        fig->x += fig->dx;
        fig->y += fig->dy;

        // Rebote en eje X
        if (fig->x < fig->x1 || fig->x > fig->x2) {
            fig->dx *= -1;
            fig->x += fig->dx; // corregir después del rebote
        }

        // Rebote en eje Y
        if (fig->y < fig->y1 || fig->y > fig->y2) {
            fig->dy *= -1;
            fig->y += fig->dy;
        }

    } else if (fig->move_type == FIG_MOVE_LINEAR) {
        // Movimiento lineal interpolado entre punto de inicio y final
        float total_time = fig->duration_ms / 100.0f; // 1 unidad = 100 ms
        float elapsed = (float)(global_time - fig->start_time);

        if (elapsed > total_time) elapsed = total_time;
        if (elapsed < 0) elapsed = 0;

        float t = elapsed / total_time;

        fig->x = fig->x_start + (int)((fig->x_end - fig->x_start) * t);
        fig->y = fig->y_start + (int)((fig->y_end - fig->y_start) * t);
    }

    // FIG_MOVE_NONE no modifica la posición
}

// Verifica si dos figuras ocupan la misma posición (colisión)
int figure_collides(const Figure *a, const Figure *b) {
    return (a->x == b->x) && (a->y == b->y);
}
