#include <stdio.h>
#include <unistd.h> 
#include "animation.h"

void print_canvas(Animation* anim) {
    char canvas[100][100];

    // Limpiar canvas
    for (int y = 0; y < anim->height; y++) {
        for (int x = 0; x < anim->width; x++) {
            canvas[y][x] = ' ';
        }
    }

    // Dibujar objetos en su posición inicial
    for (int i = 0; i < anim->object_count; i++) {
        Object obj = anim->objects[i];
        int x = obj.position.x;
        int y = obj.position.y;

        if (x >= 0 && x < anim->width && y >= 0 && y < anim->height) {
            canvas[y][x] = obj.display_char;
        }
    }

    // Imprimir canvas
    for (int y = 0; y < anim->height; y++) {
        for (int x = 0; x < anim->width; x++) {
            putchar(canvas[y][x]);
        }
        putchar('\n');
    }
}

int main() {
    Animation anim;
    parse_animation("animation.ani", &anim);

    printf("Objetos cargados: %d\n", anim.object_count);
for (int i = 0; i < anim.object_count; i++) {
    printf("Objeto %d: %s, char: %c, pos: (%d, %d)\n",
        i,
        anim.objects[i].id,
        anim.objects[i].display_char,
        anim.objects[i].position.x,
        anim.objects[i].position.y
    );
}

    print_canvas(&anim);

    return 0;
}