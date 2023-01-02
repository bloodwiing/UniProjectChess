#ifndef CHESS_VECTOR_H
#define CHESS_VECTOR_H

#include <stdint.h>
#include <stdio.h>
#include "../abstract/defs.h"
#include "teamdirection.h"

typedef struct Vector {
    int16_t x, y;
} Vector;

Vector createVector(int16_t x, int16_t y);

Vector normaliseCoordinates(int16_t x, int16_t y, TeamDirection direction);
Vector normaliseVector(Vector vector, TeamDirection direction);

bool_t compVectors(Vector a, Vector b);

#endif //CHESS_VECTOR_H
