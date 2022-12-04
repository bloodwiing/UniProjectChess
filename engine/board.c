#include "board.h"
#include "../ui/con_lib.h"
#include <string.h>
#include "../ui/render.h"
#include "validation.h"
#include <wchar.h>

Board * createEmptyBoard(Scenario * scenario, UserSettings * settings) {
    Board * out = malloc(sizeof(Board));

    out->user_settings = settings;
    out->scenario = scenario;

    out->width = scenario->size_x;
    out->height = scenario->size_y;

    out->tiles = calloc(scenario->size_x * scenario->size_y, sizeof(Tile *));
    for (int i = 0; i < scenario->size_x * scenario->size_x; ++i) {
        out->tiles[i] = calloc(1, sizeof(Tile));
    }

    out->team_count = scenario->team_count;
    out->teams = calloc(scenario->team_count, sizeof(Team));
    for (int i = 0; i < scenario->team_count; i++)
        out->teams[i] = scenario->teams[i];

    return out;
}

Board * createBoard(Scenario * scenario, UserSettings * settings) {
    Board * out = createEmptyBoard(scenario, settings);

    for (int i = 0; i < scenario->spawn_count;) {
        Spawn * spawn = scenario->spawns + i++;
        Team * team = scenario->teams + spawn->team;
        Piece * piece = team->pieces + spawn->type;
        out->tiles[spawn->x + spawn->y * scenario->size_x]->game_piece = createGamePiece(piece, spawn->type);
    }

    out->active_turn = 1;

    return out;
}

void renderBoardWithSelection(Board * board, int pos_x, int pos_y, int i, int j, int w, int h, int sel_x, int sel_y) {
    int board_width = board->width * 2 + 1,
            board_height = board->height;

    int width = w > board_width + 2 ? board_width + 2 : w,
            height = h > board_height + 2 ? board_height + 2 : h;

    if (i > board_width + 2 - width) i = board_width + 2 - width;
    if (j > board_height + 2 - height) j = board_height + 2 - height;
    int reset_i = i - 1;
    int reset_j = j - 1;

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
                renderTextColoured(board->user_settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%c", edge);

            else if (i % 2 == 1) {
                int tile = i / 2 + j * board->width;

                bool_t valid = false;
                if (sel_x != -1 && sel_y != -1)
                    valid = validatePieceAnyMove(board, sel_x, sel_y, i / 2, j);

                GamePiece * game_piece;
                if ((game_piece = board->tiles[tile]->game_piece) != NULL) {
                    renderPieceWithBackground(board->user_settings, board->teams + game_piece->team,
                                              getOriginalPiece(game_piece, board->scenario), valid ? COLOR_GREEN : COLOR_RESET);
                } else {
                    renderTextColoured(board->user_settings, valid ? COLOR_GREEN : COLOR_RESET, COLOR_LIGHT_GRAY, L" ");
                }

            } else wprintf(L" ");
        }

        wprintf(L"%*s", w - (width - reset_i) - 1, "");
    }

    for (int end = 0; end <= h - height - reset_j + 1;) {
        con_set_pos(pos_x - 1, pos_y + j + end++);
        wprintf(L"%*s", w, "");
    }
}

void renderBoard(Board * board, int pos_x, int pos_y, int i, int j, int w, int h) {
    renderBoardWithSelection(board, pos_x, pos_y, i, j, w, h, -1, -1);
}

void renderScenario(Scenario * scenario, UserSettings * settings, int pos_x, int pos_y, int i, int j, int w, int h) {
    Board * board = createBoard(scenario, settings);
    renderBoard(board, pos_x, pos_y, i, j, w, h);
    free(board);
}

void saveBoard(Board * board, FILE * stream) {
    saveScenario(board->scenario, stream);
    for (int i = 0; i < board->width * board->height;) {
        saveTile(board->tiles[i++], stream);
    }
    fwrite(&board->active_turn, sizeof(uint8_t), 1, stream);
}

Board * loadBoard(UserSettings * settings, FILE * stream) {
    Scenario * scenario = loadScenario(stream);
    Board * out = createEmptyBoard(scenario, settings);

    for (int i = 0; i < scenario->size_x * scenario->size_y; i++) {
        Tile * tile = loadTile(stream);
        if (tile->game_piece != NULL) {
            out->tiles[i]->game_piece = tile->game_piece;
        }
        free(tile);
    }

    fread(&out->active_turn, sizeof(uint8_t), 1, stream);

    return out;
}