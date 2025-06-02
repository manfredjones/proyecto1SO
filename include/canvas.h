#ifndef CANVAS_H
#define CANVAS_H

#define MAX_FIGURES 100   // Número máximo de figuras en el canvas
#define MAX_MONITORS 10   // Número máximo de monitores asociados

#include "figure.h"
#include "monitor.h"

// Estructura principal que representa el espacio de animación
typedef struct Canvas {
    int width, height;                        // Dimensiones del canvas
    Figure *figures[MAX_FIGURES];            // Figuras activas en el canvas
    int figure_count;                        // Cantidad actual de figuras
    struct Monitor *monitors[MAX_MONITORS];  // Monitores conectados
    int monitor_count;                       // Cantidad actual de monitores
} Canvas;

// Inicializa el canvas con tamaño dado y limpia figuras/monitores
void canvas_init(Canvas *canvas, int width, int height);

// Agrega una figura al canvas
void canvas_add_figure(Canvas *canvas, Figure *fig);

// Agrega un monitor al canvas
void canvas_add_monitor(Canvas *canvas, struct Monitor *mon);

// Actualiza las posiciones y colisiones de las figuras en un instante de tiempo
void canvas_update(Canvas *canvas, int current_time);

#endif
