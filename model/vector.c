#include "vector.h"

#include <stdlib.h>

Vector createVector(vec_t x, vec_t y) {
    return (Vector){.x = x, .y = y};
}

Vector normaliseCoordinates(vec_t x, vec_t y, TeamDirection direction) {
    switch (direction) {
        case TeamDirectionUp:
            return createVector(-x, -y);

        case TeamDirectionDown:
            return createVector(x, y);

        case TeamDirectionLeft:
            return createVector(-y, x);

        case TeamDirectionRight:
            return createVector(y, -x);
    }
}

Vector normaliseVector(Vector vector, TeamDirection direction) {
    return normaliseCoordinates(vector.x, vector.y, direction);
}

bool_t compVectors(Vector a, Vector b) {
    return a.x == b.x && a.y == b.y;
}
