#ifndef CHESS_SPAWN_H
#define CHESS_SPAWN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../abstract/defs.h"

typedef struct Spawn {
    ucoord_t x, y;
    team_index_t team;
    piece_index_t type;
} Spawn;

Spawn * createSpawn(ucoord_t x, ucoord_t y, team_index_t team, piece_index_t type);
void saveSpawn(Spawn * spawn, FILE * stream);
Spawn * loadSpawn(FILE * stream);
void printSpawn(Spawn * spawn);

void freeSpawn(Spawn * spawn);

#endif
