#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H

#include "board.h"
#include <stdio.h>

typedef struct GameState {
    Board * board;
    ucoord_t cur_x, cur_y;

    bool_t piece_selected;
    ucoord_t sel_x, sel_y;
} GameState;

GameState * createGameState(Board * board);
void saveGameState(GameState * game_state, FILE * stream);
GameState * loadGameState(UserSettings * settings, FILE * stream, Exception * exception);

GamePiece * getGamePieceAtCursor(GameState * state);
GamePiece * getGamePieceSelected(GameState * state);
bool_t isGameCursorOnSelected(GameState * state);

void reselectGameAtCursor(GameState * state);
void executeGameMove(GameState * state);
bool_t evaluateGameInput(GameState * state, bool_t * game_active);

#endif //CHESS_GAMESTATE_H
