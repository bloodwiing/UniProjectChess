#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "../model/scenario.h"
#include "../settings/settings.h"
#include <stdint.h>

typedef struct Board {
    UserSettings * user_settings;

    uint8_t width, height;
    Piece ** tiles;

    Team * teams;
    uint8_t team_count;
} Board;

Board * createBoard(Scenario * scenario, UserSettings * settings);
void renderBoard(Board * board, int pos_x, int pos_y, int i, int j, int w, int h);
void renderScenario(Scenario * scenario, UserSettings * settings, int pos_x, int pos_y, int i, int j, int w, int h);

#endif
