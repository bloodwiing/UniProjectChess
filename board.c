#include <string.h>
#include "board.h"

Board * createBoard(char * name, uint8_t size_x, uint8_t size_y, Team * teams, uint8_t team_count, Spawn * spawns, uint8_t spawn_count) {
    Board * out = malloc(sizeof(Board));
    strcpy(out->name, name);
    out->size_x = size_x;
    out->size_y = size_y;
    out->teams = teams;
    out->team_count = team_count;
    out->spawns = spawns;
    out->spawn_count = spawn_count;
    return out;
}

void saveBoard(Board * board, FILE * stream) {
    fwrite(board, sizeof(Board) - sizeof(Team *) - sizeof(Spawn *), 1, stream);
    for (int i = 0; i < board->team_count; i++)
        saveTeam(board->teams + i, stream);
    for (int i = 0; i < board->spawn_count; i++)
        saveSpawn(board->spawns + i, stream);
}

Board * loadBoard(FILE * stream) {
    Board * out = malloc(sizeof(Board));
    fread(out, sizeof(Board) - sizeof(Team *) - sizeof(Spawn *), 1, stream);

    out->teams = malloc(sizeof(Team) * out->team_count);
    for (int i = 0; i < out->team_count; i++)
        memcpy(out->teams + i, loadTeam(stream), sizeof(Team));

    out->spawns = malloc(sizeof(Spawn) * out->spawn_count);
    for (int i = 0; i < out->spawn_count; i++)
        memcpy(out->spawns + i, loadSpawn(stream), sizeof(Spawn));

    return out;
}

void printBoard(Board * board) {
    printf("Board: \n"
           "\tName: %s\n"
           "\tSize X: %d\n"
           "\tSize Y: %d\n"
           "\tTeams:\n",
           board->name, board->size_x, board->size_y);
    for (int i = 0; i < board->team_count; i++)
        printTeam(board->teams + i);

    printf("\tSpawns:\n");
    for (int i = 0; i < board->spawn_count; i++)
        printSpawnResolved(board->spawns + i, board);
}

Spawn * createSpawnFromPiece(Board * board, uint8_t x, uint8_t y, Piece * piece) {
    if (piece->team >= board->team_count)
        return NULL;
    Team * team = board->teams + piece->team;
    uint8_t type;
    for (type = 0; type < team->piece_count; type++)
        if (piece == team->pieces + type)
            return createSpawn(x, y, piece->team, type);
    return NULL;
}

void printSpawnResolved(Spawn * spawn, Board * board) {
    Team * team = board->teams + spawn->team;
    Piece * piece = team->pieces + spawn->type;
    printf("Spawn (Resolved): \n"
           "\tX: %d\n"
           "\tY: %d\n"
           "\tTeam: %s\n"
           "\tPiece: %s\n",
           spawn->x, spawn->y, team->name, piece->name);
}
