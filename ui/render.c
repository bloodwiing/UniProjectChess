#include "render.h"
#include "con_lib.h"
#include <stdio.h>
#include <stdarg.h>

void renderText(wchar_t * format, ...) {
    va_list argv;
    va_start(argv, format);
    vfwprintf_s(stdout, format, argv);
    va_end(argv);
}

void renderTextColoured(UserSettings * settings, int bg, int fg, wchar_t * format, ...) {
    if (settings->display.colourful)
        con_set_color(bg, fg);

    va_list argv;
    va_start(argv, format);
    vfwprintf_s(stdout, format, argv);
    va_end(argv);

    con_set_color(COLOR_RESET, COLOR_RESET);

    if (settings->display.colourful)
        con_set_color(COLOR_RESET, COLOR_RESET);
}

void renderPiece(UserSettings * settings, Team * team, Piece * piece) {
    if (settings->display.colourful)
        con_set_color(COLOR_RESET, team->colour);

    if (settings->display.unicode)
        wprintf(L"%ls", piece->unicode);
    else
        wprintf(L"%c", piece->symbol);

    if (settings->display.colourful)
        con_set_color(COLOR_RESET, COLOR_RESET);
}
