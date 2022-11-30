#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <stdint.h>
#include "team.h"
#include "spawn.h"

typedef struct Board {
    char name[20];
    uint8_t size_x, size_y;

    uint8_t team_count;
    uint8_t spawn_count;

    Team * teams;
    Spawn * spawns;
} Board;

Board * createBoard(char * name, uint8_t size_x, uint8_t size_y, Team * teams, uint8_t team_count, Spawn * spawns, uint8_t spawn_count);
void saveBoard(Board * board, FILE * stream);
Board * loadBoard(FILE * stream);
void printBoard(Board * board);

Spawn * createSpawnFromPiece(Board * board, uint8_t x, uint8_t y, Piece * piece);
void printSpawnResolved(Spawn * spawn, Board * board);

#endif
