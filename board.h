#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <stdint.h>
#include "team.h"

typedef struct Board {
    char name[20];
    uint8_t size_x, size_y;

    uint8_t team_count;
    Team * teams;
} Board;

Board * createBoard(char * name, uint8_t size_x, uint8_t size_y, Team * teams, uint8_t team_count);
void saveBoard(Board * board, FILE * stream);
Board * loadBoard(FILE * stream);
void printBoard(Board * board);

#endif
