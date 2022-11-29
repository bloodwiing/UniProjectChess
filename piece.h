#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Move {
    unsigned int x, y;
    char repeat;
} Move;

void printMove(Move move) {
    printf("x %d, y %d, repeat %s\n",
           move.x, move.y, move.repeat ? "YES" : "NO");
}

void fillMove(Move * move, unsigned char x, unsigned char y) {
    move->x = x;
    move->y = y;
}

typedef struct MoveSet {
    Move * moves;
    unsigned int move_count;

    Move * attacks;
    unsigned int attack_count;
} MoveSet;

MoveSet * createMoveSet(Move * moves, unsigned int move_count, Move * attacks, unsigned int attack_count) {
    MoveSet * out = malloc(sizeof(MoveSet));
    out->moves = moves;
    out->move_count = move_count;
    out->attacks = attacks;
    out->attack_count = attack_count;
    return out;
}

void saveMoveSet(MoveSet * move_set, FILE * stream) {
    fwrite(&move_set->move_count, sizeof(unsigned int), 1, stream);
    fwrite(move_set->moves, sizeof(Move), move_set->move_count, stream);
    fwrite(&move_set->attack_count, sizeof(unsigned int), 1, stream);
    fwrite(move_set->attacks, sizeof(Move), move_set->attack_count, stream);
}

MoveSet * loadMoveSet(FILE * stream) {
    MoveSet * out = malloc(sizeof(MoveSet));

    fread(&out->move_count, sizeof(unsigned int), 1, stream);
    out->moves = malloc(sizeof(Move) * out->move_count);
    fread(out->moves, sizeof(Move), out->move_count, stream);

    fread(&out->attack_count, sizeof(int), 1, stream);
    out->attacks = malloc(sizeof(Move) * out->attack_count);
    fread(out->attacks, sizeof(Move), out->attack_count, stream);

    return out;
}

void printMoveSet(MoveSet * move_set) {
    printf("Move Set:\n"
           "\tMoves:\n");
    for (int i = 0; i < move_set->move_count; i++) {
        printf("\t\t");
        printMove(move_set->moves[i]);
    }

    printf("\tAttacks:\n");
    for (int i = 0; i < move_set->attack_count; i++) {
        printf("\t\t");
        printMove(move_set->attacks[i]);
    }
}

typedef struct Piece {
    char name[20];  // Human-readable name of the chess piece

    char unicode[4];
    char symbol;

    unsigned char team;

    MoveSet * move_set;
} Piece;

Piece * createPiece(char * name, char * unicode, char symbol, int team, MoveSet * move_set) {
    Piece * out = malloc(sizeof(Piece));
    strcpy(out->name, name);
    strcpy(out->unicode, unicode);
    out->symbol = symbol;
    out->team = team;
    out->move_set = move_set;
    return out;
}

void savePiece(Piece * piece, FILE * stream) {
    fwrite(piece, sizeof(Piece) - sizeof(MoveSet *), 1, stream);
    saveMoveSet(piece->move_set, stream);
}

Piece * loadPiece(FILE * stream) {
    Piece * out = malloc(sizeof(Piece));
    fread(out, sizeof(Piece) - sizeof(MoveSet *), 1, stream);
    out->move_set = loadMoveSet(stream);
    return out;
}

void printPiece(Piece * piece) {
    printf("Piece: \n"
           "\tName: %s\n"
           "\tUnicode: %s\n"
           "\tSymbol: %c\n"
           "\tTeam: %d\n",
           piece->name, piece->unicode, piece->symbol, piece->team);
    printf("\tMove Set:\n");
    printMoveSet(piece->move_set);
}

typedef struct Team {
    char name[20];
    char colour[6];

    Piece * pieces;
    unsigned char piece_count;
} Team;

Team * createTeam(char * name, char * colour, Piece * pieces, unsigned char piece_count) {
    Team * out = malloc(sizeof(Team));
    strcpy(out->name, name);
    strcpy(out->colour, colour);
    out->pieces = pieces;
    out->piece_count = piece_count;
    return out;
}

void saveTeam(Team * team, FILE * stream) {
    fwrite(team, sizeof(Team) - sizeof(Piece *), 1,stream);
    fwrite(team->pieces, sizeof(Piece), team->piece_count, stream);
}

Team * loadTeam(FILE * stream) {
    Team * out = malloc(sizeof(Team));
    fread(out, sizeof(Team) - sizeof(Piece *), 1, stream);
    out->pieces = malloc(sizeof(Piece) * out->piece_count);
    fread(out->pieces, sizeof(Piece), out->piece_count, stream);
    return out;
}