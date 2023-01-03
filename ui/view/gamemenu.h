#ifndef CHESS_GAMEMENU_H
#define CHESS_GAMEMENU_H

#include "../../settings/settings.h"
#include "../../engine/board.h"
#include "../../engine/gamestate.h"

void resumeGameLoop(UserSettings * settings, GameState * game_state, bool_t save_state);
void beginNewGameLoop(UserSettings * settings, Board * board, bool_t save_state);

#endif //CHESS_GAMEMENU_H
