#ifndef CHESS_VECTOR8_H
#define CHESS_VECTOR8_H

#include <stdint.h>

#include "./teamdirection.h"
#include "./vector.h"

#include "abstract/defs.h"

#define VECTOR8_ZERO createVector8(0, 0)

#define VECTOR8_UP createVector8(0, -1)
#define VECTOR8_DOWN createVector8(0, 1)
#define VECTOR8_RIGHT createVector8(1, 0)
#define VECTOR8_LEFT createVector8(-1, 0)

typedef struct Vector8 {
    coord_t x, y;
} Vector8;

Vector8 createVector8(coord_t x, coord_t y);

Vector8 normaliseVector8(Vector8 vector, TeamDirection direction);
Vector toVector(Vector8 vector);

#endif //CHESS_VECTOR8_H
