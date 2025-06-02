#ifndef FIGURE_H
#define FIGURE_H

#include <animation.h>

typedef enum {
    FIG_MOVE_NONE,
    FIG_MOVE_BOUNCE,
    FIG_MOVE_LINEAR
} FigureMoveType;

typedef struct {
    int id;
    int x, y;
    int dx, dy;

    // Rebotar
    int x1, y1, x2, y2;

    // Lineal
    int x_start, y_start;
    int x_end, y_end;
    int duration_ms;

    FigureMoveType move_type;
    int start_time;
    int end_time;

    char symbol;
    char label[32];

    int active;
} Figure;


// Actualiza la posición de la figura considerando los límites
void figure_move(Figure *fig);

// Verifica si dos figuras colisionan
int figure_collides(const Figure *a, const Figure *b);

#endif // FIGURE_H