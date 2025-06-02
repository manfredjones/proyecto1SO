#ifndef MONITOR_H
#define MONITOR_H

#include "mypthread.h"

struct Canvas;  // Declaración adelantada para evitar dependencia circular

// Representa un monitor que observa una región del canvas
typedef struct Monitor {
    int x0, y0, x1, y1;        // Coordenadas que delimitan el área visible del monitor
    struct Canvas *canvas;    // Referencia al canvas completo
    my_mutex_t mutex;         // Mutex para sincronizar el acceso concurrente
} Monitor;

// Inicializa un monitor con su región y lo asocia al canvas correspondiente
void monitor_init(Monitor *mon, int x0, int y0, int x1, int y1, struct Canvas *canvas);

// Imprime las figuras visibles dentro del área del monitor en un instante de tiempo
void monitor_draw(Monitor *mon, int current_time);

#endif
