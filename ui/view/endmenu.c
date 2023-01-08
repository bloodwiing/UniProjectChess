#include "endmenu.h"

#include <string.h>

#include "ui/component/dialogbox.h"
#include "ui/render.h"

void showEndMenu(UserSettings * settings, Board * board, GameState * state) {
    ditherEffect();

    piece_index_t selected = 0;
    bool_t confirmed = false;

    DialogBox box;
    if (state->check)
        box = createDialogBox(L"CHECKMATE!", 0, 26, NULL);
    else
        box = createDialogBox(L"STALEMATE", 0, 28, NULL);
    box.data_ptr = board;

    drawDialogBoxCentered(settings, &box);

    con_sleep(3.0f);
    con_clear();
}

DIALOGBOX_CONTENT_CALLBACK(checkmateText) {
    Board * board = data_ptr;

    Team * winning_team = getTeam(board, (++board->active_turn) % board->team_count);

    Rect rect = createRect(x, y, width, height);
    Rect centered = centerRectInRect((int)(6 + strlen(winning_team->name)), 1, rect);

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, centered, L"%s wins!", winning_team->name);
}

DIALOGBOX_CONTENT_CALLBACK(stalemateText) {
    Rect rect = createRect(x, y, width, height);
    Rect centered = centerRectInRect(28, 1, rect);

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, centered, L"How the turns have tabled...");
}
