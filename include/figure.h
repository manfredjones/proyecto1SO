#ifndef FIGURE_H
#define FIGURE_H

#include <animation.h>

typedef struct {
    int id;
    int x, y;
    int dx, dy;
    char symbol;
    char label[32];
} Figure;

// Actualiza la posición de la figura considerando los límites
void figure_move(Figure *fig, int maxWidth, int maxHeight);

// Verifica si dos figuras colisionan
int figure_collides(const Figure *a, const Figure *b);

#endif // FIGURE_H