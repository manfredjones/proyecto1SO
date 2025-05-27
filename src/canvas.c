
#include "../include/canvas.h"
#include <stdlib.h>
#include <stdio.h>

void canvas_init(Canvas *canvas, int width, int height, int numFigures) {
    canvas->width = width;
    canvas->height = height;
    canvas->numFigures = numFigures;
    canvas->figures = malloc(sizeof(Figure) * numFigures);
}

void canvas_assign_figures(Canvas *canvas, Figure *figures, int count) {
    for (int i = 0; i < count; i++) {
        canvas->figures[i] = figures[i];
    }
}

void canvas_draw(Canvas *canvas) {
    // Función de prueba simple
    printf("Canvas: %dx%d con %d figuras\n", canvas->width, canvas->height, canvas->numFigures);
}
