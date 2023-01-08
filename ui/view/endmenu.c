#include "endmenu.h"

#include <string.h>

#include "ui/component/dialogbox.h"
#include "ui/render.h"

DIALOGBOX_CONTENT_CALLBACK(checkmateText);
DIALOGBOX_CONTENT_CALLBACK(stalemateText);

void showEndMenu(UserSettings * settings, Board * board, GameState * state) {
    ditherEffect();

    piece_index_t selected = 0;
    bool_t confirmed = false;

    DialogBox box;
    if (state->check)
        box = createDialogBox(L"CHECKMATE!", 0, 28, checkmateText);
    else
        box = createDialogBox(L"STALEMATE", 0, 28, stalemateText);
    box.data_ptr = board;

    drawDialogBoxCentered(settings, &box);

    con_sleep(3.0f);
    con_clear();
}

DIALOGBOX_CONTENT_CALLBACK(checkmateText) {
    Board * board = data_ptr;

    Team * winning_team = getTeam(board, (++board->active_turn) % board->team_count);

    Rect rect = createRect(x, y, width, height);
    Rect centered = centerRectInRect((int)(8 + strlen(winning_team->name)), 1, offsetRect(rect, 1, 0, -2, 0));
    ++centered.x;

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, centered, L"%hs wins!", winning_team->name);
}

DIALOGBOX_CONTENT_CALLBACK(stalemateText) {
    Rect rect = createRect(x, y, width, height);
    Rect centered = centerRectInRect(28, 1, rect);

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, centered, L"How the turns have tabled...");
}
