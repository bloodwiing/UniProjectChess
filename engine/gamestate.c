#include "gamestate.h"

GameState * createGameState(Board * board) {
    GameState * out = malloc(sizeof(GameState));
    out->board = board;
    out->piece_selected = false;
    out->cur_x = out->cur_y = out->sel_x = out->sel_y = 0;
    return out;
}

void saveGameState(GameState * game_state, FILE * stream) {
    saveBoard(game_state->board, stream);
}

GameState * loadGameState(UserSettings * settings, FILE * stream) {
    return createGameState(loadBoard(settings, stream));
}
