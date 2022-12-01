#include "board.h"
#include "con_lib.h"
#include <string.h>
#include "render.h"
#include <wchar.h>

Board * createBoard(Scenario * scenario, UserSettings * settings) {
    Board * out = malloc(sizeof(Board));

    out->user_settings = settings;

    out->width = scenario->size_x;
    out->height = scenario->size_y;

    out->tiles = calloc(sizeof(Piece *), scenario->size_x * scenario->size_y * sizeof(Piece *));
    for (int i = 0; i < scenario->spawn_count; i++) {
        Spawn * spawn = scenario->spawns + i;
        Team * team = scenario->teams + spawn->team;
        Piece * piece = team->pieces + spawn->type;
        out->tiles[spawn->x + spawn->y * scenario->size_x] = piece;
    }

    out->team_count = scenario->team_count;
    out->teams = malloc(sizeof(Team *) * scenario->team_count);
    for (int i = 0; i < scenario->team_count; i++)
        out->teams[i] = scenario->teams[i];

    return out;
}

void renderBoard(Board * board, int pos_x, int pos_y, int i, int j, int w, int h) {
    int board_width = board->width * 2 + 1,
        board_height = board->height;

    int width = w > board_width + 2 ? board_width + 2 : w,
        height = h > board_height + 2 ? board_height + 2 : h;

    if (i > board_width + 2 - width) i = board_width + 2 - width;
    if (j > board_height + 2 - height) j = board_height + 2 - height;
    int reset_i = i - 1;

    pos_x += 1 - i;
    pos_y += 1 - j;

    width += i - 2;
    height += j - 2;

    for (--j; j <= height; ++j) {
        for (i = reset_i; i <= width; ++i) {
            char edge = ' ';

            con_set_pos(pos_x + i, pos_y + j);
            if (i < 0 || i >= board_width)
                edge = '|';
            if (j < 0 || j >= board_height) {
                if (edge != ' ')
                    edge = '+';
                else
                    edge = '-';
            }

            if (edge != ' ')
                wprintf(L"%c", edge);

            else if (i % 2 == 1) {
                int tile = i / 2 + j * board->width;
                if (board->tiles[tile] != 0) {
                    Piece * piece = board->tiles[tile];
                    renderPiece(board->user_settings, board->teams + piece->team, piece);
                }
            }
        }
    }
}

void renderScenario(Scenario * scenario, UserSettings * settings, int pos_x, int pos_y, int i, int j, int w, int h) {
    Board * board = createBoard(scenario, settings);
    renderBoard(board, pos_x, pos_y, i, j, w, h);
    free(board);
}
