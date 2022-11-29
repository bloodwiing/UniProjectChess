#include "team.h"

Team * createTeam(char * name, char * colour, Piece * pieces, uint8_t piece_count) {
    Team * out = malloc(sizeof(Team));
    strcpy(out->name, name);
    strcpy(out->colour, colour);
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
    printf("Team: \n"
           "\tName: %s\n"
           "\tPieces:\n",
           team->name);
    for (int i = 0; i < team->piece_count; i++)
        printPiece(team->pieces + i);
}
