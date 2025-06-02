#include "object_to_figure.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Convierte un Object del lenguaje de animación en una Figure animable en el canvas
Figure* object_to_figure(const Object *obj) {
    Figure fig = {0};  // Inicializa figura con ceros
    fig.id = 0;  // ID no usado actualmente
    fig.x = obj->position.x;
    fig.y = obj->position.y;

    // Configurar tipo de movimiento según el objeto
    if (obj->movement.type == MOVE_LINEAR && obj->movement.duration > 0) {
        fig.move_type = FIG_MOVE_LINEAR;
        fig.x_start = obj->position.x;
        fig.y_start = obj->position.y;
        fig.x_end = obj->movement.to.x;
        fig.y_end = obj->movement.to.y;
        fig.duration_ms = obj->movement.duration;

    } else if (obj->movement.type == MOVE_BOUNCE) {
        fig.move_type = FIG_MOVE_BOUNCE;
        fig.dx = obj->movement.bounce.dx;
        fig.dy = obj->movement.bounce.dy;

    } else {
        fig.move_type = FIG_MOVE_NONE;
    }

    // Límites de movimiento (relevantes para rebote)
    fig.x1 = obj->movement.bounce.x1;
    fig.y1 = obj->movement.bounce.y1;
    fig.x2 = obj->movement.bounce.x2;
    fig.y2 = obj->movement.bounce.y2;

    // Configuración de tiempo de vida y símbolo visual
    fig.start_time = obj->start_time;
    fig.end_time = obj->end_time;
    fig.symbol = obj->display_char;
    fig.active = 1;

    // Copiar etiqueta del objeto como identificador de la figura
    strncpy(fig.label, obj->id, sizeof(fig.label));
    fig.label[sizeof(fig.label) - 1] = '\0';

    // Crear figura en el heap y devolver puntero
    Figure* fig_ptr = malloc(sizeof(Figure));
    if (fig_ptr) {
        *fig_ptr = fig;
    }

    printf("Figura %s creada con tiempo [%d - %d], tipo %d\n", 
           obj->id, obj->start_time, obj->end_time, obj->movement.type);
    return fig_ptr;
}
