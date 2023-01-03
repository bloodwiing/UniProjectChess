#ifndef CHESS_VECTOR_H
#define CHESS_VECTOR_H

#include <stdint.h>
#include <stdio.h>

#include "./teamdirection.h"
#include "./vector8.h"

#include "abstract/defs.h"

#define VECTOR_ZERO createVector(0, 0)

#define VECTOR_UP createVector(0, 1)
#define VECTOR_DOWN createVector(0, -1)
#define VECTOR_RIGHT createVector(1, 0)
#define VECTOR_LEFT createVector(-1, 0)

typedef int16_t vec_t;

typedef struct Vector {
    vec_t x, y;
} Vector;

Vector createVector(vec_t x, vec_t y);

Vector normaliseCoordinates(vec_t x, vec_t y, TeamDirection direction);
Vector normaliseVector(Vector vector, TeamDirection direction);
Vector normaliseVector8(Vector8 vector, TeamDirection direction);

bool_t compVectors(Vector a, Vector b);

#endif //CHESS_VECTOR_H
