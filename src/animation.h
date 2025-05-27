#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_OBJECTS 100

typedef enum {
    MOVE_LINEAR,
    MOVE_BOUNCE,
    MOVE_NONE
} MovementType;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int x1, y1, x2, y2; // Bounding box
    int dx, dy;         // Speed
} BounceInfo;

typedef struct {
    MovementType type;
    Position to;
    int duration;
    BounceInfo bounce;
} Movement;

typedef struct {
    char id[32];
    char display_char;
    int start_time;
    int end_time;
    Position position;
    Movement movement;
    Position current_pos;
} Object;

typedef struct {
    int width;
    int height;
    int object_count;
    Object objects[MAX_OBJECTS];
} Animation;

void parse_animation(const char* filename, Animation* anim);
void run_animation(Animation* anim);

#endif