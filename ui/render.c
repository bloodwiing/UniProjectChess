#include "render.h"
#include "con_lib.h"
#include <stdio.h>

void renderPiece(UserSettings * settings, Team * team, Piece * piece) {
    if (settings->display.colourful == true) {
        con_set_color(COLOR_BLACK, team->colour);
    }

    if (settings->display.unicode == true) {
        wprintf(L"%ls", piece->unicode);
    } else {
        wprintf(L"%c", piece->symbol);
    }

    con_set_color(COLOR_BLACK, COLOR_GRAY);
}
