#include "team.h"

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define TEAM_STRUCT_SIZE_WITHOUT_POINTERS offsetof(Team, piece_count) + sizeof(count_t)

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

    uint8_t direction = team.direction;
    fwrite(&direction, sizeof(uint8_t), 1, stream);

    for (piece_index_t i = 0; i < team.piece_count;)
        savePiece(team.pieces[i++], stream);
}

Team loadTeam(FILE * stream) {
    Team out = {};
    fread(&out, TEAM_STRUCT_SIZE_WITHOUT_POINTERS, 1, stream);

    uint8_t direction;
    fread(&direction, sizeof(uint8_t), 1, stream);
    out.direction = direction;

    out.pieces = calloc(out.piece_count, sizeof(Piece));
    for (piece_index_t i = 0; i < out.piece_count;)
        out.pieces[i++] = loadPiece(stream);

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

    normaliseMoveSet(&team->pieces[team->piece_count - 1].move_set, team->direction);
}

void addPromotion(Team * team, piece_index_t type) {
    team->promotions = realloc(team->promotions, sizeof(piece_index_t) * ++team->promotion_count);
    team->promotions[team->promotion_count - 1] = type;
}

void clearPromotions(Team * team) {
    free(team->promotions);
    team->promotions = NULL;
    team->promotion_count = 0;
}

void freeTeam(Team * team) {
    for (piece_index_t i = 0; i < team->piece_count;)
        freeMoveSet(&team->pieces[i++].move_set);
    free(team->promotions);
    free(team->pieces);
}
