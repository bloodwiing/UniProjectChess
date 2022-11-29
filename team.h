#ifndef CHESS_TEAM_H
#define CHESS_TEAM_H

#include <stdio.h>
#include <stdint.h>
#include "piece.h"

typedef struct Team {
    char name[20];
    char colour[6];

    uint8_t piece_count;
    Piece * pieces;
} Team;

Team * createTeam(char * name, char * colour, Piece * pieces, uint8_t piece_count);
void saveTeam(Team * team, FILE * stream);
Team * loadTeam(FILE * stream);
void printTeam(Team * team);

#endif
