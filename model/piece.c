#include "piece.h"

#include <string.h>
#include <stdlib.h>

#define STRUCT_PIECE_SIZE_WITHOUT_MOVE_SET offsetof(Piece, symbol) + sizeof(char)

Piece createPiece(char * name, wchar_t * unicode, char symbol, bool_t upgradable, bool_t protect, team_index_t team, MoveSet move_set) {
    Piece out = {
        .symbol = symbol,
        .upgradable = upgradable,
        .protect = protect,
        .team = team,
        .move_set = move_set
    };
    strcpy(out.name, name);
    wcscpy(out.unicode, unicode);
    return out;
}

void savePiece(Piece piece, FILE * stream) {
    fwrite(&piece, STRUCT_PIECE_SIZE_WITHOUT_MOVE_SET, 1, stream);

    wchar16_t * converted = createU16(piece.unicode, PIECE_UNICODE_LENGTH);
    fwrite(converted, sizeof(wchar16_t), PIECE_UNICODE_LENGTH, stream);
    free(converted);

    saveMoveSet(piece.move_set, stream);
}

Piece loadPiece(FILE * stream) {
    Piece out = {};
    fread(&out, STRUCT_PIECE_SIZE_WITHOUT_MOVE_SET, 1, stream);

    wchar16_t unicode[PIECE_UNICODE_LENGTH];
    fread(unicode, sizeof(wchar16_t), PIECE_UNICODE_LENGTH, stream);

    wchar_t * converted = createWStr(unicode, PIECE_UNICODE_LENGTH);
    memcpy(out.unicode, converted, sizeof(wchar_t) * PIECE_UNICODE_LENGTH);
    free(converted);

    out.move_set = loadMoveSet(stream);

    return out;
}

void printPiece(Piece piece) {
    wprintf(L"Piece: \n"
             "\tName: %s\n"
             "\tUnicode: %ls\n"
             "\tSymbol: %c\n"
             "\tUpgradable: %s\n"
             "\tProtect: %s\n"
             "\tTeam: %d\n"
             "\tMove Set:\n",
             piece.name, piece.unicode, piece.symbol, piece.upgradable ? "Yes" : "No",
             piece.protect ? "Yes" : "No", piece.team);
    printMoveSet(piece.move_set);
}

void freePiece(Piece * piece) {
    freeMoveSet(&piece->move_set);
}
