
#ifndef CANVAS_H
#define CANVAS_H

#include "figure.h"
#include "monitor.h"

typedef struct {
    int width;
    int height;
    int numFigures;
    Figure *figures;
} Canvas;

void canvas_init(Canvas *canvas, int width, int height, int numFigures);
void canvas_assign_figures(Canvas *canvas, Figure *figures, int count);
void canvas_draw(Canvas *canvas);

#endif // CANVAS_H
