#include "object_to_figure.h"
#include <string.h>

Figure object_to_figure(const Object *obj) {
    Figure fig;
    fig.id = 0;
    fig.x = obj->position.x;
    fig.y = obj->position.y;
    fig.dx = obj->movement.bounce.dx;
    fig.dy = obj->movement.bounce.dy;
    fig.symbol = obj->display_char;
    strncpy(fig.label, obj->id, sizeof(fig.label));
    fig.label[sizeof(fig.label) - 1] = '\0';
    return fig;
}
