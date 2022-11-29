#include "piece.h"

void initPiece(Piece * piece, char * name, char * unicode, char symbol, uint8_t team, MoveSet * move_set) {
    strcpy(piece->name, name);
    strcpy(piece->unicode, unicode);
    piece->symbol = symbol;
    piece->team = team;
    piece->move_set = move_set;
}

void savePiece(Piece * piece, FILE * stream) {
    fwrite(piece, sizeof(Piece) - sizeof(MoveSet *), 1, stream);
    saveMoveSet(piece->move_set, stream);
}

void loadPiece(Piece * piece, FILE * stream) {
    fread(piece, sizeof(Piece) - sizeof(MoveSet *), 1, stream);
    piece->move_set = loadMoveSet(stream);
}

void printPiece(Piece * piece) {
    printf("Piece: \n"
           "\tName: %s\n"
           "\tUnicode: %s\n"
           "\tSymbol: %c\n"
           "\tTeam: %d\n"
           "\tMove Set:\n",
           piece->name, piece->unicode, piece->symbol, piece->team);
    printMoveSet(piece->move_set);
}
