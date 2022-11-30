#include "move.h"
#include <stdio.h>

void initMove(Move * move, int8_t x, int8_t y, bool_t repeat) {
    move->x = x;
    move->y = y;
    move->repeat = repeat;
}

void printMove(Move move) {
    wprintf(L"x %d, y %d, repeat %s\n",
             move.x, move.y, move.repeat == true ? "YES" : "NO");
}
