#ifndef PARSER_H
#define PARSER_H

#include "figure.h"
#include "animation.h"

Figure* object_to_figure(const Object *obj);

int parse_animation(const char* filename, Animation* anim);

#endif
