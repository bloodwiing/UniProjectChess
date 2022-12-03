#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H

#include "../ui/board.h"

typedef struct GameState {
    Board * board;
    uint8_t cur_x, cur_y;

    bool_t piece_selected;
    uint8_t sel_x, sel_y;
} GameState;

GameState * createGameState(Board * board);

#endif //CHESS_GAMESTATE_H
