#include "../include/animation.h"
#include "../include/figure.h"
#include "../include/object_to_figure.h"
#include <stdio.h>

int main() {
    Object obj = {
        .id = "Pelota",
        .display_char = 'O',
        .start_time = 0,
        .end_time = 10,
        .position = {5, 5},
        .movement = {
            .type = MOVE_BOUNCE,
            .bounce = { .dx = 1, .dy = 1 }
        }
    };

    Figure fig = object_to_figure(&obj);

    printf("Figura convertida:\n");
    printf("Label: %s\n", fig.label);
    printf("Símbolo: %c\n", fig.symbol);
    printf("Posición: (%d, %d)\n", fig.x, fig.y);
    printf("Movimiento: dx = %d, dy = %d\n", fig.dx, fig.dy);

    return 0;
}
