#include "team.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

Team * createTeam(char * name, uint8_t colour, Piece * pieces, uint8_t piece_count) {
    Team * out = malloc(sizeof(Team));
    strcpy(out->name, name);
    out->colour = colour;
    out->pieces = pieces;
    out->piece_count = piece_count;
    return out;
}

void saveTeam(Team * team, FILE * stream) {
    fwrite(team, sizeof(Team) - sizeof(Piece *), 1,stream);
    for (int i = 0; i < team->piece_count; i++)
        savePiece(team->pieces + i, stream);
}

Team * loadTeam(FILE * stream) {
    Team * out = malloc(sizeof(Team));
    fread(out, sizeof(Team) - sizeof(Piece *), 1, stream);
    out->pieces = malloc(sizeof(Piece) * out->piece_count);
    for (int i = 0; i < out->piece_count; i++)
        loadPiece(out->pieces + i, stream);
    return out;
}

void printTeam(Team * team) {
    wprintf(L"Team: \n"
             "\tName: %s\n"
             "\tPieces:\n",
             team->name);
    for (int i = 0; i < team->piece_count; i++)
        printPiece(team->pieces + i);
}
