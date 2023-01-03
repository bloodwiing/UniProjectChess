#include "spawn.h"
#include <wchar.h>

Spawn * createSpawn(ucoord_t x, ucoord_t y, team_index_t team, piece_index_t type) {
    Spawn * out = malloc(sizeof(Spawn));
    out->x = x;
    out->y = y;
    out->team = team;
    out->type = type;
    return out;
}

void saveSpawn(Spawn * spawn, FILE * stream) {
    fwrite(spawn, sizeof(Spawn), 1, stream);
}

Spawn * loadSpawn(FILE * stream) {
    Spawn * out = malloc(sizeof(Spawn));
    fread(out, sizeof(Spawn), 1, stream);
    return out;
}

void printSpawn(Spawn * spawn) {
    wprintf(L"Spawn: \n"
             "\tX: %d\n"
             "\tY: %d\n"
             "\tTeam: %d\n"
             "\tPiece: %d",
             spawn->x, spawn->y, spawn->team, spawn->type);
}

void freeSpawn(Spawn * spawn) {
    free(spawn);
}
