#ifndef CHESS_VECTOR_H
#define CHESS_VECTOR_H

#include <stdint.h>
#include <stdio.h>
#include "../abstract/defs.h"
#include "teamdirection.h"

typedef int16_t vec_t;

typedef struct Vector {
    vec_t x, y;
} Vector;

Vector createVector(vec_t x, vec_t y);

Vector normaliseCoordinates(vec_t x, vec_t y, TeamDirection direction);
Vector normaliseVector(Vector vector, TeamDirection direction);

bool_t compVectors(Vector a, Vector b);

#endif //CHESS_VECTOR_H
