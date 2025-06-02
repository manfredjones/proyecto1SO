#ifndef ANIMATION_H
#define ANIMATION_H

#define MAX_OBJECTS 100  // Máximo número de objetos en una animación

// Tipos de movimiento que un objeto puede tener
typedef enum {
    MOVE_LINEAR,   // Movimiento recto de un punto a otro
    MOVE_BOUNCE,   // Movimiento con rebote entre límites
    MOVE_NONE      // Sin movimiento (estático)
} MovementType;

// Representa una posición (x, y) en el canvas
typedef struct {
    int x;
    int y;
} Position;

// Información específica para movimiento de rebote
typedef struct {
    int x1, y1, x2, y2; // Límites de rebote (caja)
    int dx, dy;         // Velocidad en cada eje
} BounceInfo;

// Información de movimiento asociada a un objeto
typedef struct {
    MovementType type;  // Tipo de movimiento
    Position to;        // Destino (para MOVE_LINEAR)
    int duration;       // Duración del movimiento lineal
    BounceInfo bounce;  // Parámetros para MOVE_BOUNCE
} Movement;

// Estructura que representa un objeto animado
typedef struct {
    char id[32];           // Identificador textual
    char display_char;     // Carácter a mostrar en pantalla
    int start_time;        // Tiempo en el que entra en escena
    int end_time;          // Tiempo en el que sale de escena
    Position position;     // Posición inicial
    Movement movement;     // Tipo y datos del movimiento
    Position current_pos;  // Posición actual (actualizada durante la ejecución)
} Object;

// Estructura principal que contiene los datos de la animación
typedef struct {
    int width;                // Ancho del canvas
    int height;               // Alto del canvas
    int object_count;         // Número de objetos cargados
    Object objects[MAX_OBJECTS];  // Arreglo de objetos animables
} Animation;

// Parsea un archivo .ani y llena la estructura Animation
int parse_animation(const char* filename, Animation* anim);

// Ejecuta la animación secuencialmente (modo no concurrente)
void run_animation(Animation* anim);

#endif
