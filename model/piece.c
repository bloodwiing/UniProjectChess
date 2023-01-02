#include "piece.h"
#include <string.h>
#include <stdlib.h>

void initPiece(Piece * piece, char * name, wchar_t * unicode, char symbol, bool_t upgradable, bool_t protect, uint8_t team, MoveSet * move_set) {
    strcpy(piece->name, name);
    wcscpy(piece->unicode, unicode);
    piece->symbol = symbol;
    piece->upgradable = upgradable;
    piece->protect = protect;
    piece->team = team;
    piece->move_set = move_set;
}

void savePiece(Piece * piece, FILE * stream) {
    fwrite(piece, sizeof(Piece) - sizeof(MoveSet *) - sizeof(wchar_t) * PIECE_UNICODE_LENGTH, 1, stream);
    fwrite(createU16(piece->unicode, PIECE_UNICODE_LENGTH), sizeof(wchar16_t), PIECE_UNICODE_LENGTH, stream);
    saveMoveSet(piece->move_set, stream);
}

void loadPiece(Piece * piece, FILE * stream) {
    fread(piece, sizeof(Piece) - sizeof(MoveSet *) - sizeof(wchar_t) * PIECE_UNICODE_LENGTH, 1, stream);
    wchar16_t name[PIECE_UNICODE_LENGTH];
    fread(name, sizeof(wchar16_t), PIECE_UNICODE_LENGTH, stream);
    memcpy(piece->unicode, createWStr(name, PIECE_UNICODE_LENGTH), sizeof(wchar_t) * PIECE_UNICODE_LENGTH);
    piece->move_set = loadMoveSet(stream);
}

void printPiece(Piece * piece) {
    wprintf(L"Piece: \n"
             "\tName: %s\n"
             "\tUnicode: %ls\n"
             "\tSymbol: %c\n"
             "\tUpgradable: %s\n"
             "\tProtect: %s\n"
             "\tTeam: %d\n"
             "\tMove Set:\n",
             piece->name, piece->unicode, piece->symbol, piece->upgradable ? "Yes" : "No", piece->protect ? "Yes" : "No", piece->team);
    printMoveSet(piece->move_set);
}

void freePiece(Piece * piece) {
    freeMoveSet(piece->move_set);
    free(piece);
}
