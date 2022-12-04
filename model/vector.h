#ifndef CHESS_VECTOR_H
#define CHESS_VECTOR_H

#include <stdint.h>
#include <stdio.h>
#include "teamdirection.h"

typedef struct Vector {
    int16_t x, y;
} Vector;

Vector * createVector(int16_t x, int16_t y);
void saveVector(Vector * vector, FILE * stream);
Vector * loadVector(FILE * stream);
Vector * normaliseCoordinates(int16_t x, int16_t y, TeamDirection direction);
Vector * normaliseVector(Vector * vector, TeamDirection direction);

#endif //CHESS_VECTOR_H
