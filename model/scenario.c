#include <string.h>
#include "scenario.h"
#include <wchar.h>

#define STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS sizeof(Scenario) - sizeof(Team *) - sizeof(Spawn *)

Scenario * createScenario(char * name, char * author, ucoord_t size_x, ucoord_t size_y, Team * teams, team_index_t team_count, Spawn * spawns, spawn_index_t spawn_count) {
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
    for (team_index_t i = 0; i < scenario->team_count; i++)
        saveTeam(scenario->teams + i, stream);
    for (spawn_index_t i = 0; i < scenario->spawn_count; i++)
        saveSpawn(scenario->spawns + i, stream);
}

Scenario * loadScenario(FILE * stream) {
    Scenario * out = malloc(sizeof(Scenario));
    fread(out, STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS, 1, stream);

    out->teams = malloc(sizeof(Team) * out->team_count);
    for (team_index_t i = 0; i < out->team_count; i++) {
        Team * team = loadTeam(stream);
        memcpy(out->teams + i, team, sizeof(Team));
        free(team);
    }

    out->spawns = malloc(sizeof(Spawn) * out->spawn_count);
    for (spawn_index_t i = 0; i < out->spawn_count; i++) {
        Spawn * spawn = loadSpawn(stream);
        memcpy(out->spawns + i, spawn, sizeof(Spawn));
        free(spawn);
    }

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
    for (team_index_t i = 0; i < scenario->team_count; i++)
        printTeam(scenario->teams + i);

    wprintf(L"\tSpawns:\n");
    for (spawn_index_t i = 0; i < scenario->spawn_count; i++)
        printSpawnResolved(scenario->spawns + i, scenario);
}

Spawn * createSpawnFromPiece(Scenario * scenario, ucoord_t x, ucoord_t y, Piece * piece) {
    if (piece->team >= scenario->team_count)
        return NULL;
    Team * team = scenario->teams + piece->team;
    for (piece_index_t type = 0; type < team->piece_count; type++)
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

void freeScenario(Scenario * scenario) {
    for (team_index_t i = 0; i < scenario->team_count;) {
        Team * team = scenario->teams + i++;
        for (piece_index_t j = 0; j < team->piece_count;)
            freeMoveSet(team->pieces[j++].move_set);
        free(team->pieces);
    }
    free(scenario->teams);

    free(scenario->spawns);
    free(scenario);
}
