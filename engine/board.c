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

Team * getTeam(Board * board, int index) {
    if (index < 0 || index >= board->team_count)
        return NULL;
    return board->teams + index;
}

Team * getPieceTeam(Board * board, Piece * piece) {
    return getTeam(board, piece->team);
}

Team * getGamePieceTeam(Board * board, GamePiece * piece) {
    return getTeam(board, piece->team);
}

Tile * getTile(Board * board, int x, int y) {
    return board->tiles[x + board->width * y];
}

GamePiece * getBoardGamePiece(Board * board, int x, int y) {
    return getTile(board, x, y)->game_piece;
}

Piece * getTilePiece(Board * board, Tile * tile) {
    if (tile->game_piece == NULL)
        return NULL;
    return getOriginalPiece(tile->game_piece, board->scenario);
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
