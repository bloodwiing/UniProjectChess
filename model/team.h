#ifndef CHESS_TEAM_H
#define CHESS_TEAM_H

#include <stdio.h>
#include <stdint.h>
#include "piece.h"
#include "teamdirection.h"
#include "../ui/con_lib.h"

struct GamePiece;

#define TEAM_NAME_LENGTH 20

typedef struct Team {
    char name[TEAM_NAME_LENGTH];
    colour_t colour;

    count_t piece_count;
    Piece * pieces;

    TeamDirection direction;

    struct GamePiece * protected_piece;
} Team;

Team * createTeam(char * name, colour_t colour, Piece * pieces, count_t piece_count, TeamDirection direction);
void saveTeam(Team * team, FILE * stream);
Team * loadTeam(FILE * stream);
void printTeam(Team * team);

void freeTeam(Team * team);

#endif
