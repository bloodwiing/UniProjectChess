#ifndef CHESS_SPAWN_H
#define CHESS_SPAWN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Spawn {
    uint8_t x, y;
    uint8_t team, type;
} Spawn;

Spawn * createSpawn(uint8_t x, uint8_t y, uint8_t team, uint8_t type);
void saveSpawn(Spawn * spawn, FILE * stream);
Spawn * loadSpawn(FILE * stream);
void printSpawn(Spawn * spawn);

#endif
