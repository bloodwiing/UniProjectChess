#include <string.h>
#include "scenario.h"
#include <wchar.h>
#include "../abstract/version.h"

#define STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS sizeof(Scenario) - sizeof(Team *) - sizeof(Spawn *)

Scenario * createScenario(char * name, char * author, uint8_t size_x, uint8_t size_y, Team * teams, uint8_t team_count, Spawn * spawns, uint16_t spawn_count) {
    Scenario * out = calloc(1, sizeof(Scenario));
    out->version = BUILD_VERSION;
    strncpy(out->name, name, SCENARIO_MAX_STRING_LEN);
    strncpy(out->author, author, SCENARIO_MAX_STRING_LEN);
    out->size_x = size_x;
    out->size_y = size_y;
    out->teams = teams;
    out->team_count = team_count;
    out->spawns = spawns;
    out->spawn_count = spawn_count;
    return out;
}

void saveScenario(Scenario * scenario, FILE * stream) {
    fwrite(scenario, STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS, 1, stream);
    for (int i = 0; i < scenario->team_count; i++)
        saveTeam(scenario->teams + i, stream);
    for (int i = 0; i < scenario->spawn_count; i++)
        saveSpawn(scenario->spawns + i, stream);
}

Scenario * loadScenario(FILE * stream) {
    Scenario * out = malloc(sizeof(Scenario));
    fread(out, STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS, 1, stream);

    out->teams = malloc(sizeof(Team) * out->team_count);
    for (int i = 0; i < out->team_count; i++)
        memcpy(out->teams + i, loadTeam(stream), sizeof(Team));

    out->spawns = malloc(sizeof(Spawn) * out->spawn_count);
    for (int i = 0; i < out->spawn_count; i++)
        memcpy(out->spawns + i, loadSpawn(stream), sizeof(Spawn));

    return out;
}

void printScenario(Scenario * scenario) {
    wprintf(L"Scenario: \n"
             "\tVersion: %hs\n"
             "\tName: %hs\n"
             "\tAuthor: %hs\n"
             "\tSize X: %d\n"
             "\tSize Y: %d\n"
             "\tTeams:\n",
             getVersionName(scenario->version), scenario->name, scenario->author, scenario->size_x, scenario->size_y);
    for (int i = 0; i < scenario->team_count; i++)
        printTeam(scenario->teams + i);

    wprintf(L"\tSpawns:\n");
    for (int i = 0; i < scenario->spawn_count; i++)
        printSpawnResolved(scenario->spawns + i, scenario);
}

Spawn * createSpawnFromPiece(Scenario * scenario, uint8_t x, uint8_t y, Piece * piece) {
    if (piece->team >= scenario->team_count)
        return NULL;
    Team * team = scenario->teams + piece->team;
    uint8_t type;
    for (type = 0; type < team->piece_count; type++)
        if (piece == team->pieces + type)
            return createSpawn(x, y, piece->team, type);
    return NULL;
}

void printSpawnResolved(Spawn * spawn, Scenario * scenario) {
    Team * team = scenario->teams + spawn->team;
    Piece * piece = team->pieces + spawn->type;
    wprintf(L"Spawn (Resolved): \n"
             "\tX: %d\n"
             "\tY: %d\n"
             "\tTeam: %hs\n"
             "\tPiece: %hs\n",
             spawn->x, spawn->y, team->name, piece->name);
}
