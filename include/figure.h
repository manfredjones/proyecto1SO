#ifndef FIGURE_H
#define FIGURE_H

#include <animation.h>

// Tipos de movimiento interno de una figura (independiente del Object original)
typedef enum {
    FIG_MOVE_NONE,     // Sin movimiento
    FIG_MOVE_BOUNCE,   // Rebote dentro de un área
    FIG_MOVE_LINEAR    // Movimiento recto entre dos puntos
} FigureMoveType;

// Representa una figura animada dentro del canvas
typedef struct {
    int id;            // ID numérico interno (no usado actualmente)

    int x, y;          // Posición actual
    int dx, dy;        // Velocidad actual (usado en rebote)

    // Parámetros de rebote: límites de la región donde rebota
    int x1, y1, x2, y2;

    // Parámetros de movimiento lineal
    int x_start, y_start;  // Punto inicial
    int x_end, y_end;      // Punto final
    int duration_ms;       // Duración del movimiento en milisegundos

    FigureMoveType move_type; // Tipo de movimiento asignado

    int start_time;       // Momento en que la figura aparece
    int end_time;         // Momento en que la figura desaparece

    char symbol;          // Carácter que representa la figura
    char label[32];       // Etiqueta textual (copiada del Object original)

    int active;           // Estado: 1 si está activa, 0 si fue desactivada
} Figure;

// Mueve la figura según su tipo y el tiempo global actual
void figure_move(Figure *fig);

// Retorna 1 si dos figuras ocupan la misma posición, 0 en caso contrario
int figure_collides(const Figure *a, const Figure *b);

#endif // FIGURE_H
