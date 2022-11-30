#include "board.h"
#include "con_lib.h"

Board * createBoard(Scenario * scenario) {
    Board * out = malloc(sizeof(Board));
    out->width = scenario->size_x;
    out->height = scenario->size_y;
    out->tiles = calloc(sizeof(Piece *), scenario->size_x * scenario->size_y);
    return out;
}

void renderBoard(Board * board, int pos_x, int pos_y, int x, int y, int w, int h) {
    con_clear();

    int width = w > board->width + 2 ? board->width + 2 : w,
        height = h > board->height + 2 ? board->height + 2 : h;

    int real_x = x > width - board->width ? width - board->width : x,
        real_y = y > height - board->height ? height - board->height : y;

    ++pos_x, ++pos_y;
    --width; --height;

    for (int i = x - 1; i <= width; ++i) {
        for (int j = y - 1; j <= height; j++) {
            con_set_pos(pos_x + i, pos_y + j);
            if (i == -1 || i == height || j == -1 || j == width) {
                printf("#");
            }
        }
    }
}

void renderScenario(Scenario * scenario, int x, int y, int w, int h) {
    Board * board = createBoard(scenario);
    renderBoard(board, 0, 0, x, y, w, h);
    free(board);
}
