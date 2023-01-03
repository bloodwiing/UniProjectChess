#include "team.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define TEAM_STRUCT_SIZE_WITHOUT_POINTERS sizeof(Team) - sizeof(Piece *) - sizeof(TeamDirection) - sizeof(struct GamePiece *)

Team * createTeam(char * name, colour_t colour, Piece * pieces, count_t piece_count, TeamDirection direction) {
    Team * out = calloc(1, sizeof(Team));
    strcpy(out->name, name);
    out->colour = colour;
    out->pieces = pieces;
    out->piece_count = piece_count;
    out->direction = direction;

    out->protected_piece = NULL;
    return out;
}

void saveTeam(Team * team, FILE * stream) {
    fwrite(team, TEAM_STRUCT_SIZE_WITHOUT_POINTERS, 1,stream);

    for (piece_index_t i = 0; i < team->piece_count; i++)
        savePiece(team->pieces + i, stream);

    fwrite((uint8_t *) &team->direction, 1, 1, stream);
}

Team * loadTeam(FILE * stream) {
    Team * out = malloc(sizeof(Team));
    fread(out, TEAM_STRUCT_SIZE_WITHOUT_POINTERS, 1, stream);

    out->pieces = malloc(sizeof(Piece) * out->piece_count);
    for (piece_index_t i = 0; i < out->piece_count; i++)
        loadPiece(out->pieces + i, stream);

    uint8_t direction;
    fread(&direction, 1, 1, stream);
    out->direction = direction;

    out->protected_piece = NULL;
    return out;
}

void printTeam(Team * team) {
    wprintf(L"Team: \n"
             "\tName: %hs\n"
             "\tDirection: %d\n"
             "\tPieces:\n",
             team->name, team->direction);
    for (int i = 0; i < team->piece_count; i++)
        printPiece(team->pieces + i);
}

void freeTeam(Team * team) {
    for (piece_index_t i = 0; i < team->piece_count;)
        freeMoveSet(team->pieces[i++].move_set);
    free(team->pieces);
    free(team);
}
