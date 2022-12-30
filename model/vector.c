#include <stdlib.h>
#include "vector.h"

Vector createVector(int16_t x, int16_t y) {
    return (Vector){.x = x, .y = y};
}

Vector normaliseCoordinates(int16_t x, int16_t y, TeamDirection direction) {
    switch (direction) {
        case TeamDirectionUp:
            return createVector(x, y);

        case TeamDirectionDown:
            return createVector(-x, -y);

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
