#include <stdbool.h>
#include "defaults.h"

void initDefaultPawn(Piece * piece, uint8_t team) {
    // move possibilities
    Move * moves = malloc(sizeof(Move) * 1);
    initMove(moves + 0, 0, 1, false);

    // attack possibilities
    Move * attacks = malloc(sizeof(Move) * 2);
    initMove(attacks + 0, 1, 1, false);
    initMove(attacks + 1, -1, 1, false);

    // compile move set
    MoveSet * move_set = malloc(sizeof(MoveSet));
    initMoveSet(move_set, moves, 1, attacks, 2);

    // create the piece
    initPiece(piece, "Pawn", team == 1 ? "♙" : "♟", team == 1 ? 'p' : 'P', team, move_set);
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
    initMoveSet(move_set, moves, 4, moves, 4);

    // create the piece
    initPiece(piece, "Rook", team == 1 ? "♖" : "♜", team == 1 ? 'r' : 'R', team, move_set);
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
    initMoveSet(move_set, moves, 8, moves, 8);

    // create the piece
    initPiece(piece, "Knight", team == 1 ? "♘" : "♞", team == 1 ? 'k' : 'K', team, move_set);
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
    initMoveSet(move_set, moves, 4, moves, 4);

    // create the piece
    initPiece(piece, "Rook", team == 1 ? "♖" : "♜", team == 1 ? 'r' : 'R', team, move_set);
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
    initMoveSet(move_set, moves, 8, moves, 8);

    // create the piece
    initPiece(piece, "Queen", team == 1 ? "♕" : "♛", team == 1 ? 'q' : 'Q', team, move_set);
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
    initMoveSet(move_set, moves, 8, moves, 8);

    // create the piece
    initPiece(piece, "King", team == 1 ? "♔" : "♚", team == 1 ? 'k' : 'K', team, move_set);
}

Team * createDefaultTeamWhite() {
    Piece * pieces = calloc(6, sizeof(Piece));
    initDefaultPawn(pieces + 0, 1);
    initDefaultRook(pieces + 1, 1);
    initDefaultKnight(pieces + 2, 1);
    initDefaultBishop(pieces + 3, 1);
    initDefaultQueen(pieces + 4, 1);
    initDefaultKing(pieces + 5, 1);

    return createTeam("White", "\x1B[37m", pieces, 6);
}

Team * createDefaultTeamBlack() {
    Piece * pieces = calloc(6, sizeof(Piece));
    initDefaultPawn(pieces + 0, 0);
    initDefaultRook(pieces + 1, 0);
    initDefaultKnight(pieces + 2, 0);
    initDefaultBishop(pieces + 3, 0);
    initDefaultQueen(pieces + 4, 0);
    initDefaultKing(pieces + 5, 0);

    return createTeam("Black", "\x1B[30m", pieces, 6);
}

Board * createDefaultBoard() {
    Team * teams = calloc(2, sizeof(Team));
    memcpy(teams + 0, createDefaultTeamBlack(), sizeof(Team));
    memcpy(teams + 1, createDefaultTeamWhite(), sizeof(Team));
    return createBoard("Normal", 8, 8, teams, 2);
}