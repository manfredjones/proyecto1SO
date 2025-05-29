#include <stdio.h>
#include <unistd.h> // para usleep
#include <string.h> // para memset
#include "../include/figure.h"

#define WIDTH 20
#define HEIGHT 10
#define NUM_FIGURES 2

void draw_figures(Figure figs[], int n, int width, int height) {
    char grid[HEIGHT][WIDTH + 1]; // +1 para el '\0' al final de cada fila

    // Inicializar grid con '.'
    for (int i = 0; i < height; i++) {
        memset(grid[i], '.', width);
        grid[i][width] = '\0';
    }

    // Colocar figuras en el grid
    for (int i = 0; i < n; i++) {
        int x = figs[i].x;
        int y = figs[i].y;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = figs[i].symbol;
        }
    }

    // Imprimir grid
    for (int i = 0; i < height; i++) {
        printf("%s\n", grid[i]);
    }
}

int main() {
    Figure figs[NUM_FIGURES];

    // Inicializar figuras
    figs[0].id = 1; figs[0].x = 5;  figs[0].y = 5;  figs[0].dx = 1; figs[0].dy = 0; figs[0].symbol = 'A';
    figs[1].id = 2; figs[1].x = 15; figs[1].y = 5;  figs[1].dx = -1; figs[1].dy = 0; figs[1].symbol = 'B';

    for (int t = 0; t < 30; t++) {
        printf("Frame %d:\n", t);

        // Mover figuras
        for (int i = 0; i < NUM_FIGURES; i++) {
            figure_move(&figs[i], WIDTH, HEIGHT);
        }

        draw_figures(figs, NUM_FIGURES, WIDTH, HEIGHT);

        // Checar colisiones
        for (int i = 0; i < NUM_FIGURES; i++) {
            for (int j = i + 1; j < NUM_FIGURES; j++) {
                if (figure_collides(&figs[i], &figs[j])) {
                    printf("¡Colisión entre %c y %c!\n", figs[i].symbol, figs[j].symbol);
                }
            }
        }

        printf("\n");
        usleep(300 * 1000); // 300 ms para ver el movimiento
    }

    return 0;
}