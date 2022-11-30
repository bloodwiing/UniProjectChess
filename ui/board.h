#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "../model/scenario.h"
#include <stdint.h>

typedef struct Board {
    Piece *** tiles;
    uint8_t width, height;
} Board;

Board * createBoard(Scenario * scenario);
void renderBoard(Board * board, int pos_x, int pos_y, int x, int y, int w, int h);
void renderScenario(Scenario * scenario, int x, int y, int w, int h);

#endif
