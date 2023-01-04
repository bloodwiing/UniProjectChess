#include "vector8.h"

Vector8 createVector8(coord_t x, coord_t y) {
    return (Vector8){.x = x, .y = y};
}

Vector8 normaliseVector8(Vector8 vector, TeamDirection direction) {
    Vector new = normaliseCoordinates(vector.x, vector.y, direction);
    return createVector8((coord_t)new.x, (coord_t)new.y);
}

Vector toVector(Vector8 vector) {
    return createVector(vector.x, vector.y);
}
