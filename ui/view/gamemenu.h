#ifndef CHESS_GAMEMENU_H
#define CHESS_GAMEMENU_H

#include "engine/board.h"
#include "engine/gamestate.h"

#include "settings/settings.h"

/// Resumes a GameState
/// \param settings Display Settings
/// \param game_state The active Game State
/// \param save_state Flag to save the Game State once finished
void resumeGameLoop(UserSettings * settings, GameState * game_state, bool_t save_state);

/// Starts a new GameState
/// \param settings Display Settings
/// \param board The Board of the game to begin
/// \param save_state Flag to save the Game State once finished
void beginNewGameLoop(UserSettings * settings, Board * board, bool_t save_state);

#endif //CHESS_GAMEMENU_H
