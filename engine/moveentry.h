#ifndef CHESS_MOVEENTRY_H
#define CHESS_MOVEENTRY_H

#include "abstract/defs.h"

#include "engine/gamepiece.h"

#include "model/piece.h"

typedef uint8_t move_flag_t;

#define MOVE_ENTRY_NORMAL 0x0
#define MOVE_ENTRY_CAPTURE 0x1
#define MOVE_ENTRY_PHANTOM 0x2
#define MOVE_ENTRY_PROMOTION 0x4
#define MOVE_ENTRY_DRAW_OFFER 0x8
#define MOVE_ENTRY_CHECK 0x10
#define MOVE_ENTRY_CHECKMATE 0x20
#define MOVE_ENTRY_AMBIGUOUS_FILE 0x40
#define MOVE_ENTRY_AMBIGUOUS_RANK 0x80

typedef struct MoveEntry {
    ucoord_t from_x, from_y, to_x, to_y;
    GamePiece * game_piece;
    Piece * type;

    move_flag_t flags;

    SpecialMove * phantom_move;

    Piece * new_type;

    SpecialMove * special_move;
} MoveEntry;

MoveEntry createMoveEntry(Piece * type, GamePiece * game_piece, ucoord_t from_x, ucoord_t from_y, ucoord_t to_x, ucoord_t to_y);

void addMoveEntryFlag(MoveEntry * entry, move_flag_t flag);

void markMoveEntrySpecialMove(MoveEntry * entry, SpecialMove * special_move);
void markMoveEntryPromote(MoveEntry * entry, Piece * new_type);
void markMoveEntryPhantom(MoveEntry * entry, SpecialMove * phantom_move);
void markMoveEntryCheck(MoveEntry * entry, bool_t is_checkmate);
void markMoveEntryAmbiguous(MoveEntry * entry, bool_t file, bool_t rank);

#endif //CHESS_MOVEENTRY_H
