#include "moveentry.h"

MoveEntry createMoveEntry(Piece * type, GamePiece * game_piece, ucoord_t from_x, ucoord_t from_y, ucoord_t to_x, ucoord_t to_y) {
    return (MoveEntry){
        .type = type,
        .game_piece = game_piece,
        .from_x = from_x,
        .from_y = from_y,
        .to_x = to_x,
        .to_y = to_y,
        .flags = MOVE_ENTRY_NORMAL
    };
}

void addMoveEntryFlag(MoveEntry * entry, move_flag_t flag) {
    entry->flags |= flag;
}

void markMoveEntrySpecialMove(MoveEntry * entry, SpecialMove * special_move) {
    entry->special_move = special_move;
}

void markMoveEntryPromote(MoveEntry * entry, Piece * new_type) {
    addMoveEntryFlag(entry, MOVE_ENTRY_PROMOTION);
    entry->new_type = new_type;
}

void markMoveEntryPhantom(MoveEntry * entry, SpecialMove * phantom_move) {
    addMoveEntryFlag(entry, MOVE_ENTRY_PHANTOM);
    entry->phantom_move = phantom_move;
}

void markMoveEntryCheck(MoveEntry * entry, bool_t is_checkmate) {
    addMoveEntryFlag(entry, MOVE_ENTRY_CHECK | (is_checkmate ? MOVE_ENTRY_CHECKMATE : MOVE_ENTRY_NORMAL));
}

void markMoveEntryAmbiguous(MoveEntry * entry, bool_t file, bool_t rank) {
    addMoveEntryFlag(entry, (file ? MOVE_ENTRY_AMBIGUOUS_FILE : MOVE_ENTRY_NORMAL) | (rank ? MOVE_ENTRY_AMBIGUOUS_RANK : MOVE_ENTRY_NORMAL));
}
