#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "canvas.h"
#include "monitor.h"
#include "figure.h"

int global_time = 0;
Canvas canvas;

void canvas_init(Canvas* c, int width, int height) {
    c->width = width;
    c->height = height;
    c->figure_count = 0;
    c->monitor_count = 0;
}

void canvas_add_figure(Canvas* c, Figure* f) {
    if (c->figure_count < MAX_FIGURES) {
        c->figures[c->figure_count++] = f;
    }
}

void canvas_add_monitor(Canvas* c, Monitor* m) {
    if (c->monitor_count < MAX_MONITORS) {
        c->monitors[c->monitor_count++] = m;
    }
}

void canvas_update(Canvas* c, int time) {
    // Mover figuras activas
    for (int i = 0; i < c->figure_count; i++) {
        Figure* fig = c->figures[i];
        if (!fig->active) continue; 
        if (time >= fig->start_time && time <= fig->end_time) {
            figure_move(fig);
        }
        if (fig->x < 0 || fig->x >= c->width || fig->y < 0 || fig->y >= c->height) {
            fig->active = 0;
        }
    }

    // Detectar colisiones y desactivar
    for (int i = 0; i < c->figure_count; i++) {
        Figure* a = c->figures[i];
        if (!a->active) continue;

        for (int j = i + 1; j < c->figure_count; j++) {
            Figure* b = c->figures[j];
            if (!b->active) continue;

            if (a->x == b->x && a->y == b->y) {
                a->active = 0;
                b->active = 0;
            }
        }
    }
}



void canvas_draw(Canvas *canvas, int current_time, int monitor_id) {
    Monitor *mon = canvas->monitors[monitor_id];
    int width = mon->x1 - mon->x0 + 1;
    int height = mon->y1 - mon->y0 + 1;

    char buffer[height][width];
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            buffer[y][x] = ' ';

    for (int i = 0; i < canvas->figure_count; i++) {
        Figure *f = canvas->figures[i];
        if (f->active && current_time >= f->start_time && current_time <= f->end_time)
            continue; // Solo dibujar figuras activas en el tiempo actual
        if (current_time >= f->start_time && current_time <= f->end_time) {
            if (f->x >= mon->x0 && f->x <= mon->x1 && f->y >= mon->y0 && f->y <= mon->y1) {
                int local_x = f->x - mon->x0;
                int local_y = f->y - mon->y0;
                buffer[local_y][local_x] = f->symbol;
            }
        }
    }

    printf("Monitor (%d,%d) - (%d,%d) at time %d:\n", mon->x0, mon->y0, mon->x1, mon->y1, current_time);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            putchar(buffer[y][x]);
        }
        putchar('\n');
    }
}

void draw_full_canvas(Canvas *canvas, int current_time) {

    char screen[canvas->height][canvas->width];
    for (int y = 0; y < canvas->height; y++)
        for (int x = 0; x < canvas->width; x++)
            screen[y][x] = ' ';
    
    int col_div = canvas->width / 2;
    int row_div = canvas->height / 2;

    for (int y = 0; y < canvas->height; y++) {
        screen[y][col_div] = '|';
    }
    for (int x = 0; x < canvas->width; x++) {
        screen[row_div][x] = '-';
    }
    screen[row_div][col_div] = '+';  // intersección



    int **collision_map = malloc(canvas->height * sizeof(int *));
    for (int y = 0; y < canvas->height; y++) {
        collision_map[y] = calloc(canvas->width, sizeof(int));
    }

    for (int i = 0; i < canvas->figure_count; i++) {
        Figure *f = canvas->figures[i];
        if (f->active && current_time >= f->start_time && current_time <= f->end_time) {
            if (f->x >= 0 && f->x < canvas->width && f->y >= 0 && f->y < canvas->height) {
                if (collision_map[f->y][f->x] == 0) {
                    screen[f->y][f->x] = f->symbol;
                } else {
                    screen[f->y][f->x] = 'B'; // Boom!
                }
                collision_map[f->y][f->x]++;
            }
        }
    }
    for (int y = 0; y < canvas->height; y++) {
        free(collision_map[y]);
    }
    free(collision_map);


    printf("\033[2J\033[H"); // Limpiar pantalla
    printf("Frame %d:\n", current_time);

    // Borde superior
    putchar('+');
    for (int x = 0; x < canvas->width; x++) putchar('-');
    putchar('+');
    putchar('\n');

    // Contenido con bordes laterales
    for (int y = 0; y < canvas->height; y++) {
        putchar('|');
        for (int x = 0; x < canvas->width; x++) {
            putchar(screen[y][x]);
        }
        putchar('|');
        putchar('\n');
    }

    // Borde inferior
    putchar('+');
    for (int x = 0; x < canvas->width; x++) putchar('-');
    putchar('+');
    putchar('\n');
}

