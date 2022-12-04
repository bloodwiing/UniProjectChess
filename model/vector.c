#include <stdlib.h>
#include "vector.h"

Vector * createVector(int16_t x, int16_t y) {
    Vector * out = calloc(1, sizeof(Vector));
    out->x = x;
    out->y = y;
}

void saveVector(Vector * vector, FILE * stream) {
    fwrite(vector, sizeof(Vector), 1, stream);
}

Vector * loadVector(FILE * stream) {
    Vector * out = malloc(sizeof(Vector));
    fread(out, sizeof(Vector), 1, stream);
    return out;
}

Vector * normaliseCoordinates(int16_t x, int16_t y, TeamDirection direction) {
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

Vector * normaliseVector(Vector * vector, TeamDirection direction) {
    return normaliseCoordinates(vector->x, vector->y, direction);
}
