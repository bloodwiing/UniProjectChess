#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <stdbool.h>

typedef struct Move {
    int x, y;
    bool repeat;
} Move;

void initMove(Move * move, int x, int y, bool repeat);
void printMove(Move move);

#endif
