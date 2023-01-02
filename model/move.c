#include "move.h"
#include <wchar.h>
#include <stdlib.h>

void initMove(Move * move, Vector vector, bool_t repeat) {
    move->vector = vector;
    move->repeat = repeat;
}

void initMoveRaw(Move * move, int8_t x, int8_t y, bool_t repeat) {
    initMove(move, (Vector){.x = x, .y = y}, repeat);
}

void printMove(Move move) {
    wprintf(L"x %d, y %d, repeat %s\n",
             move.vector.x, move.vector.y, move.repeat == true ? "YES" : "NO");
}

void freeMove(Move * move) {
    free(move);
}