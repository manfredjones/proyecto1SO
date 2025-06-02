#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "animation.h"
#include "parser.h"
#include "object_to_figure.h"


int parse_animation(const char* filename, Animation* anim) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir archivo");
        exit(1);
    }

    char line[256];
    Object current;
    int in_object = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strncmp(line, "CANVAS", 6) == 0) {
            sscanf(line, "CANVAS %d %d", &anim->width, &anim->height);
            
        } else if (strncmp(line, "OBJECT", 6) == 0) {
                if (in_object) {
                    current.current_pos = current.position;
                    anim->objects[anim->object_count++] = current;
                }
                in_object = 1;

                // Mover lectura del ID antes del memset
                char temp_id[32];
                sscanf(line, "OBJECT %s", temp_id);

                memset(&current, 0, sizeof(Object));
                strncpy(current.id, temp_id, sizeof(current.id) - 1);
                current.id[sizeof(current.id) - 1] = '\0';
                current.movement.type = MOVE_NONE;
        } else if (strncmp(line, "CHAR", 4) == 0) {
            sscanf(line, "CHAR %c", &current.display_char);
        } else if (strncmp(line, "START", 5) == 0) {
            sscanf(line, "START %d", &current.start_time);
            printf("   ↳ START leído: %d\n", current.start_time);
        } else if (strncmp(line, "END", 3) == 0) {
            sscanf(line, "END %d", &current.end_time);
            printf("   ↳ END leído: %d\n", current.end_time);
        } else if (strncmp(line, "POSITION", 8) == 0) {
            sscanf(line, "POSITION %d %d", &current.position.x, &current.position.y);
        } else if (strncmp(line, "MOVE TO", 7) == 0) {
            current.movement.type = MOVE_LINEAR;
            sscanf(line, "MOVE TO %d %d DURATION %d", &current.movement.to.x, &current.movement.to.y, &current.movement.duration);
        } else if (strncmp(line, "BOUNCE", 6) == 0) {
            current.movement.type = MOVE_BOUNCE;
            sscanf(line, "BOUNCE LIMITS %d %d %d %d SPEED %d %d",
                   &current.movement.bounce.x1, &current.movement.bounce.y1,
                   &current.movement.bounce.x2, &current.movement.bounce.y2,
                   &current.movement.bounce.dx, &current.movement.bounce.dy);
        }
    }

    if (in_object) {
        current.current_pos = current.position;
        anim->objects[anim->object_count++] = current;
    }

    fclose(file);
    return anim->object_count;
}

void draw_canvas(Animation* anim, int time) {
    char canvas[anim->height][anim->width];
    memset(canvas, ' ', sizeof(canvas));

    for (int i = 0; i < anim->object_count; i++) {
        Object* obj = &anim->objects[i];
        if (time >= obj->start_time && time <= obj->end_time) {
            int x = obj->current_pos.x;
            int y = obj->current_pos.y;
            if (x >= 0 && x < anim->width && y >= 0 && y < anim->height) {
                canvas[y][x] = obj->display_char;
            }
        }
    }

    printf("\033[2J\033[H"); // Limpiar pantalla

    for (int y = 0; y < anim->height; y++) {
        for (int x = 0; x < anim->width; x++) {
            putchar(canvas[y][x]);
        }
        putchar('\n');
    }
}

void update_objects(Animation* anim, int time) {
    for (int i = 0; i < anim->object_count; i++) {
        Object* obj = &anim->objects[i];
        if (time < obj->start_time || time > obj->end_time) continue;

        if (obj->movement.type == MOVE_LINEAR) {
            float progress = (float)(time - obj->start_time) / obj->movement.duration;
            if (progress > 1.0f) progress = 1.0f;
            obj->current_pos.x = obj->position.x + (int)((obj->movement.to.x - obj->position.x) * progress);
            obj->current_pos.y = obj->position.y + (int)((obj->movement.to.y - obj->position.y) * progress);
        } else if (obj->movement.type == MOVE_BOUNCE) {
            obj->current_pos.x += obj->movement.bounce.dx;
            obj->current_pos.y += obj->movement.bounce.dy;

            if (obj->current_pos.x <= obj->movement.bounce.x1 || obj->current_pos.x >= obj->movement.bounce.x2)
                obj->movement.bounce.dx *= -1;
            if (obj->current_pos.y <= obj->movement.bounce.y1 || obj->current_pos.y >= obj->movement.bounce.y2)
                obj->movement.bounce.dy *= -1;
        }
    }
}

void run_animation(Animation* anim) {
    int time = 0;
    int max_time = 0;

    for (int i = 0; i < anim->object_count; i++) {
        if (anim->objects[i].end_time > max_time)
            max_time = anim->objects[i].end_time;
    }

    while (time <= max_time) {
        update_objects(anim, time);
        draw_canvas(anim, time);
        usleep(100 * 1000); // 100ms por frame
        time += 100;
    }
}