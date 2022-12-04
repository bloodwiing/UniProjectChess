#include "defaults.h"
#include <string.h>
#include "model/defs.h"
#include "ui/con_lib.h"

void initDefaultPawn(Piece * piece, uint8_t team) {
    // move possibilities
    Move * moves = malloc(sizeof(Move));
    initMove(moves + 0, 0, 1, false);

    // attack possibilities
    Move * attacks = malloc(sizeof(Move) * 2);
    initMove(attacks + 0, 1, 1, false);
    initMove(attacks + 1, -1, 1, false);

    // special possibilities
    SpecialMove * specials = malloc(sizeof(SpecialMove));
    initSpecialMoveVulnerable(specials + 0, 0, 2, true, 0, -1);

    // compile move set
    MoveSet * move_set = malloc(sizeof(MoveSet));
    initMoveSet(move_set, moves, 1, attacks, 2, specials, 1);

    // create the piece
    initPiece(piece, "Pawn", team == 1 ? L"♟" : L"♙", team == 1 ? 'p' : 'P', true, false, team, move_set);
}

void initDefaultRook(Piece * piece, uint8_t team) {
    // move possibilities
    Move * moves = malloc(sizeof(Move) * 4);
    initMove(moves + 0, 0, 1, true);
    initMove(moves + 1, 0, -1, true);
    initMove(moves + 2, 1, 0, true);
    initMove(moves + 3, -1, 0, true);

    // compile move set
    MoveSet * move_set = malloc(sizeof(MoveSet));
    initMoveSet(move_set, moves, 4, moves, 4, NULL, 0);

    // create the piece
    initPiece(piece, "Rook", team == 1 ? L"♜" : L"♖", team == 1 ? 'r' : 'R', false, false, team, move_set);
}

void initDefaultKnight(Piece * piece, uint8_t team) {
    // move possibilities
    Move * moves = malloc(sizeof(Move) * 8);
    initMove(moves + 0, 1, 2, false);
    initMove(moves + 1, 1, -2, false);

    initMove(moves + 2, -1, 2, false);
    initMove(moves + 3, -1, -2, false);

    initMove(moves + 4, 2, 1, false);
    initMove(moves + 5, -2, 1, false);

    initMove(moves + 6, 2, -1, false);
    initMove(moves + 7, -2, -1, false);

    // compile move set
    MoveSet * move_set = malloc(sizeof(MoveSet));
    initMoveSet(move_set, moves, 8, moves, 8, NULL, 0);

    // create the piece
    initPiece(piece, "Knight", team == 1 ? L"♞" : L"♘", team == 1 ? 'n' : 'N', false, false, team, move_set);
}

void initDefaultBishop(Piece * piece, uint8_t team) {
    // move possibilities
    Move * moves = malloc(sizeof(Move) * 4);
    initMove(moves + 0, 1, 1, true);
    initMove(moves + 1, -1, 1, true);
    initMove(moves + 2, 1, -1, true);
    initMove(moves + 3, -1, -1, true);

    // compile move set
    MoveSet * move_set = malloc(sizeof(MoveSet));
    initMoveSet(move_set, moves, 4, moves, 4, NULL, 0);

    // create the piece
    initPiece(piece, "Rook", team == 1 ? L"♝" : L"♗", team == 1 ? 'b' : 'B', false, false, team, move_set);
}

void initDefaultQueen(Piece * piece, uint8_t team) {
    // move possibilities
    Move * moves = malloc(sizeof(Move) * 8);
    initMove(moves + 0, 0, 1, true);
    initMove(moves + 1, 0, -1, true);
    initMove(moves + 2, 1, 0, true);
    initMove(moves + 3, -1, 0, true);
    initMove(moves + 4, 1, 1, true);
    initMove(moves + 5, -1, 1, true);
    initMove(moves + 6, 1, -1, true);
    initMove(moves + 7, -1, -1, true);

    // compile move set
    MoveSet * move_set = malloc(sizeof(MoveSet));
    initMoveSet(move_set, moves, 8, moves, 8, NULL, 0);

    // create the piece
    initPiece(piece, "Queen", team == 1 ? L"♛" : L"♕", team == 1 ? 'q' : 'Q', false, false, team, move_set);
}

void initDefaultKing(Piece * piece, uint8_t team) {
    // move possibilities
    Move * moves = malloc(sizeof(Move) * 8);
    initMove(moves + 0, 0, 1, false);
    initMove(moves + 1, 0, -1, false);
    initMove(moves + 2, 1, 0, false);
    initMove(moves + 3, -1, 0, false);
    initMove(moves + 4, 1, 1, false);
    initMove(moves + 5, -1, 1, false);
    initMove(moves + 6, 1, -1, false);
    initMove(moves + 7, -1, -1, false);

    // compile move set
    MoveSet * move_set = malloc(sizeof(MoveSet));
    initMoveSet(move_set, moves, 8, moves, 8, NULL, 0);

    // create the piece
    initPiece(piece, "King", team == 1 ? L"♚" : L"♔", team == 1 ? 'k' : 'K', false, true, team, move_set);
}

Team * createDefaultTeamWhite() {
    Piece * pieces = calloc(6, sizeof(Piece));
    initDefaultPawn(pieces + 0, 1);
    initDefaultRook(pieces + 1, 1);
    initDefaultKnight(pieces + 2, 1);
    initDefaultBishop(pieces + 3, 1);
    initDefaultQueen(pieces + 4, 1);
    initDefaultKing(pieces + 5, 1);

    return createTeam("White", COLOR_LIGHT_ORANGE, pieces, 6, TeamDirectionDown);
}

Team * createDefaultTeamBlack() {
    Piece * pieces = calloc(6, sizeof(Piece));
    initDefaultPawn(pieces + 0, 0);
    initDefaultRook(pieces + 1, 0);
    initDefaultKnight(pieces + 2, 0);
    initDefaultBishop(pieces + 3, 0);
    initDefaultQueen(pieces + 4, 0);
    initDefaultKing(pieces + 5, 0);

    return createTeam("Black", COLOR_RED, pieces, 6, TeamDirectionUp);
}

Scenario * createDefaultScenario() {
    Team * teams = calloc(2, sizeof(Team));
    memcpy(teams + 0, createDefaultTeamBlack(), sizeof(Team));
    memcpy(teams + 1, createDefaultTeamWhite(), sizeof(Team));

    Spawn * spawns = calloc(32, sizeof(Spawn));
    spawns[0] = *createSpawn(0, 0, 0, 1);  // black rook
    spawns[1] = *createSpawn(1, 0, 0, 2);  // black knight
    spawns[2] = *createSpawn(2, 0, 0, 3);  // black bishop
    spawns[3] = *createSpawn(3, 0, 0, 4);  // black queen
    spawns[4] = *createSpawn(4, 0, 0, 5);  // black king
    spawns[5] = *createSpawn(5, 0, 0, 3);  // black bishop
    spawns[6] = *createSpawn(6, 0, 0, 2);  // black knight
    spawns[7] = *createSpawn(7, 0, 0, 1);  // black rook
    for (int i = 0; i < 8; i++) {
        spawns[8+i] = *createSpawn(i, 1, 0, 0);  // black pawn
        spawns[16+i] = *createSpawn(i, 6, 1, 0);  // white pawn
    }
    spawns[24] = *createSpawn(0, 7, 1, 1);  // white rook
    spawns[25] = *createSpawn(1, 7, 1, 2);  // white knight
    spawns[26] = *createSpawn(2, 7, 1, 3);  // white bishop
    spawns[27] = *createSpawn(3, 7, 1, 4);  // white queen
    spawns[28] = *createSpawn(4, 7, 1, 5);  // white king
    spawns[29] = *createSpawn(5, 7, 1, 3);  // white bishop
    spawns[30] = *createSpawn(6, 7, 1, 2);  // white knight
    spawns[31] = *createSpawn(7, 7, 1, 1);  // white rook

    return createScenario("Normal", "BLOODWIING", 8, 8, teams, 2, spawns, 32);
}