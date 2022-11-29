#include "move.h"
#include <stdio.h>
#include <stdbool.h>

void initMove(Move * move, int x, int y, bool repeat) {
    move->x = x;
    move->y = y;
    move->repeat = repeat;
}

void printMove(Move move) {
    printf("x %d, y %d, repeat %s\n",
           move.x, move.y, move.repeat == true ? "YES" : "NO");
}
