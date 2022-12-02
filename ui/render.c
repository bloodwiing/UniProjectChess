#include "render.h"
#include "con_lib.h"
#include <stdio.h>

void renderText(wchar_t * text) {
    wprintf(L"%ls", text);
}

void renderTextColoured(UserSettings * settings, int fg, int bg, wchar_t * text) {
    if (settings->display.colourful == true)
        con_set_color(bg, fg);

    renderText(text);

    if (settings->display.colourful == true)
        con_set_color(COLOR_RESET, COLOR_RESET);
}

void renderPiece(UserSettings * settings, Team * team, Piece * piece) {
    if (settings->display.colourful == true)
        con_set_color(COLOR_RESET, team->colour);

    if (settings->display.unicode)
        wprintf(L"%ls", piece->unicode);
    else
        wprintf(L"%c", piece->symbol);

    if (settings->display.colourful == true)
        con_set_color(COLOR_RESET, COLOR_RESET);
}
