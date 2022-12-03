#include "render.h"
#include "con_lib.h"
#include <stdio.h>
#include <stdarg.h>

void renderText(wchar_t * format, ...) {
    va_list argv;
    va_start(argv, format);
    vfwprintf(stdout, format, argv);
    va_end(argv);
}

void renderTextColoured(UserSettings * settings, int bg, int fg, wchar_t * format, ...) {
    if (settings->display.colourful)
        con_set_color(bg, fg);

    va_list argv;
    va_start(argv, format);
    vfwprintf(stdout, format, argv);
    va_end(argv);

    if (settings->display.colourful)
        con_set_color(COLOR_RESET, COLOR_RESET);
}

void renderPieceWithBackground(UserSettings * settings, Team * team, Piece * piece, int bg) {
    if (settings->display.unicode)
        renderTextColoured(settings, bg, team->colour, L"%ls", piece->unicode);
    else
        renderTextColoured(settings, bg, team->colour, L"%c", piece->symbol);
}

void renderPiece(UserSettings * settings, Team * team, Piece * piece) {
    renderPieceWithBackground(settings, team, piece, COLOR_RESET);
}

void clearRect(int x, int y, int w, int h) {
    for (int i = y; i < y + h;) {
        con_set_pos(x, i++);
        wprintf(L"%*s", w, "");
    }
}
