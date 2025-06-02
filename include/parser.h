#ifndef PARSER_H
#define PARSER_H

#include "figure.h"
#include "animation.h"

// Carga una animación desde archivo .ani
int parse_animation(const char* filename, Animation* anim);

#endif
