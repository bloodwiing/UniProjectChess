#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H

#include "board.h"
#include <stdio.h>

#define GAME_STATE_SAVE_FILE "./data/save.bin"

typedef struct GameState {
    Board * board;
    ucoord_t cur_x, cur_y;

    bool_t piece_selected;
    ucoord_t sel_x, sel_y;
} GameState;

GameState * createGameState(Board * board);
void saveGameState(GameState * game_state, FILE * stream);
GameState * loadGameState(UserSettings * settings, FILE * stream, Exception * exception);

void saveGameStateDefault(GameState * game_state);
GameState * loadGameStateDefault(UserSettings * settings, Exception * exception);

GamePiece * getGamePieceAtCursor(GameState * state);
GamePiece * getGamePieceSelected(GameState * state);
bool_t isGameCursorOnSelected(GameState * state);

void reselectGameAtCursor(GameState * state);
void executeGameMove(GameState * state);
bool_t evaluateGameInput(GameState * state, bool_t * game_active);

#endif //CHESS_GAMESTATE_H
