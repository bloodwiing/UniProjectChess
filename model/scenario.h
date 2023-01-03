#ifndef CHESS_SCENARIO_H
#define CHESS_SCENARIO_H

#include <stdint.h>
#include "team.h"
#include "spawn.h"
#include "../abstract/version.h"

#define SCENARIO_FOLDER "./scenario"
#define SCENARIO_MAX_STRING_LEN 20

typedef struct Scenario {
    version_t version;

    char name[SCENARIO_MAX_STRING_LEN];
    char author[SCENARIO_MAX_STRING_LEN];
    ucoord_t size_x, size_y;

    team_index_t team_count;
    spawn_index_t spawn_count;

    Team * teams;
    Spawn * spawns;
} Scenario;

Scenario * createScenario(char * name, char * author, ucoord_t size_x, ucoord_t size_y, Team * teams, team_index_t team_count, Spawn * spawns, spawn_index_t spawn_count);
void saveScenario(Scenario * scenario, FILE * stream);
Scenario * loadScenario(FILE * stream);
void printScenario(Scenario * scenario);

Spawn * createSpawnFromPiece(Scenario * scenario, ucoord_t x, ucoord_t y, Piece * piece);
void printSpawnResolved(Spawn * spawn, Scenario * scenario);

void freeScenario(Scenario * scenario);

#endif
