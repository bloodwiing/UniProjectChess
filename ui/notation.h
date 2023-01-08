#ifndef CHESS_NOTATION_H
#define CHESS_NOTATION_H

#include "engine/moveentry.h"

#include "settings/settings.h"

/// Returns the text representation of a tile's X
/// \return The File string (remember to free it!)
char * getFileNotation(uint8_t x);

/// Returns the text representation of a tile's Y
/// \return The Rank string (remember to free it!)
char * getRankNotation(uint8_t y);


/// Generates a representative string (Algebraic notation) for a given Move entry
/// \param settings Display settings
/// \param entry The move entry
/// \return The move string (remember to free it!)
wchar_t * generateMoveNotation(UserSettings * settings, MoveEntry entry);

#endif //CHESS_NOTATION_H
