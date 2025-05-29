#include "../include/figure.h"

void figure_move(Figure *fig, int maxWidth, int maxHeight) {
    fig->x += fig->dx;
    fig->y += fig->dy;

    // Rebote en los bordes
    if (fig->x < 0) {
        fig->x = 0;
        fig->dx *= -1;
    } else if (fig->x >= maxWidth) {
        fig->x = maxWidth - 1;
        fig->dx *= -1;
    }

    if (fig->y < 0) {
        fig->y = 0;
        fig->dy *= -1;
    } else if (fig->y >= maxHeight) {
        fig->y = maxHeight - 1;
        fig->dy *= -1;
    }
}

int figure_collides(const Figure *a, const Figure *b) {
    return (a->x == b->x) && (a->y == b->y);
}