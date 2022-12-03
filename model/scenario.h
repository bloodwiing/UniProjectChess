#ifndef CHESS_SCENARIO_H
#define CHESS_SCENARIO_H

#include <stdint.h>
#include "team.h"
#include "spawn.h"

#define SCENARIO_FOLDER "./scenario"
#define SCENARIO_MAX_STRING_LEN 20

typedef struct Scenario {
    char name[SCENARIO_MAX_STRING_LEN];
    char author[SCENARIO_MAX_STRING_LEN];
    uint8_t size_x, size_y;

    uint8_t team_count;
    uint16_t spawn_count;

    Team * teams;
    Spawn * spawns;
} Scenario;

Scenario * createScenario(char * name, char * author, uint8_t size_x, uint8_t size_y, Team * teams, uint8_t team_count, Spawn * spawns, uint16_t spawn_count);
void saveScenario(Scenario * scenario, FILE * stream);
Scenario * loadScenario(FILE * stream);
void printScenario(Scenario * scenario);

Spawn * createSpawnFromPiece(Scenario * scenario, uint8_t x, uint8_t y, Piece * piece);
void printSpawnResolved(Spawn * spawn, Scenario * scenario);

#endif
