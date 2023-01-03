#include "team.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define TEAM_STRUCT_SIZE_WITHOUT_POINTERS sizeof(Team) - sizeof(Piece *) - sizeof(TeamDirection) - sizeof(struct GamePiece *)

Team createTeam(char * name, colour_t colour, TeamDirection direction) {
    Team out = {
        .colour = colour,
        .direction = direction
    };
    strncpy(out.name, name, TEAM_NAME_LENGTH - 1);
    return out;
}

void saveTeam(Team team, FILE * stream) {
    fwrite(&team, TEAM_STRUCT_SIZE_WITHOUT_POINTERS, 1,stream);

    for (piece_index_t i = 0; i < team.piece_count;)
        savePiece(team.pieces[i++], stream);

    fwrite((uint8_t *) &team.direction, 1, 1, stream);
}

Team loadTeam(FILE * stream) {
    Team out = {};
    fread(&out, TEAM_STRUCT_SIZE_WITHOUT_POINTERS, 1, stream);

    out.pieces = calloc(out.piece_count, sizeof(Piece));
    for (piece_index_t i = 0; i < out.piece_count;)
        out.pieces[i++] = loadPiece(stream);

    uint8_t direction;
    fread(&direction, 1, 1, stream);
    out.direction = direction;

    return out;
}

void printTeam(Team team) {
    wprintf(L"Team: \n"
             "\tName: %hs\n"
             "\tDirection: %d\n"
             "\tPieces:\n",
             team.name, team.direction);
    for (int i = 0; i < team.piece_count;)
        printPiece(team.pieces[i++]);
}

void addPiece(Team * team, Piece piece) {
    team->pieces = realloc(team->pieces, sizeof(Piece) * ++team->piece_count);
    memset(&team->pieces[team->piece_count - 1], 0, sizeof(Piece));
    team->pieces[team->piece_count - 1] = piece;
}

void freeTeam(Team * team) {
    for (piece_index_t i = 0; i < team->piece_count;)
        freeMoveSet(&team->pieces[i++].move_set);
    free(team->pieces);
}
