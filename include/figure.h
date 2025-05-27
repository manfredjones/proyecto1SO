
#ifndef FIGURE_H
#define FIGURE_H

typedef struct {
    int id;
    int x, y;
    int dx, dy; // dirección del movimiento
    char symbol;
} Figure;

void figure_move(Figure *fig, int maxWidth, int maxHeight);
int figure_collides(Figure *a, Figure *b);

#endif // FIGURE_H
