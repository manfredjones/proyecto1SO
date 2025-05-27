
#include "../include/figure.h"

void figure_move(Figure *fig, int maxWidth, int maxHeight) {
    fig->x += fig->dx;
    fig->y += fig->dy;

    if (fig->x < 0 || fig->x >= maxWidth) fig->dx *= -1;
    if (fig->y < 0 || fig->y >= maxHeight) fig->dy *= -1;
}

int figure_collides(Figure *a, Figure *b) {
    return a->x == b->x && a->y == b->y;
}
