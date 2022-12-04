#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H

#include "board.h"
#include <stdio.h>

typedef struct GameState {
    Board * board;
    uint8_t cur_x, cur_y;

    bool_t piece_selected;
    uint8_t sel_x, sel_y;
} GameState;

GameState * createGameState(Board * board);
void saveGameState(GameState * game_state, FILE * stream);
GameState * loadGameState(UserSettings * settings, FILE * stream);

#endif //CHESS_GAMESTATE_H
