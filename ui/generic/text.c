#include "text.h"

#include <stdlib.h>

#include "ui/con_lib.h"

#include "utils/encoding.h"

size_t getWordLen(wchar_t * string) {
    if (string == NULL)
        return -1;

    size_t len = 0;
    while (*string && *(string++) != L' ')
        ++len;
    return len;
}

void renderText(const wchar_t * format, ...) {
    va_list argv;
    va_start(argv, format);
    vfwprintf(stdout, format, argv);
    va_end(argv);
}

void renderTextColoured(UserSettings * settings, int bg, int fg, const wchar_t * format, ...) {
    if (settings->display.colourful)
        con_set_color(bg, fg);

    va_list argv;
    va_start(argv, format);
    vfwprintf(stdout, format, argv);
    va_end(argv);

    if (settings->display.colourful)
        con_set_color(COLOR_RESET, COLOR_RESET);
}

void renderTextColouredWrappedRect(UserSettings * settings, int bg, int fg, Rect rect, const wchar_t * format, ...) {
    if (settings->display.colourful)
        con_set_color(bg, fg);

    // Rect's width and height is the distance between the edges, not how much it takes up
    // Why? I don't remember, but changing it breaks a lot of rendering :)
    --rect.width;
    --rect.height;

    va_list argv1, argv2;
    va_start(argv1, format);
    va_copy(argv2, argv1);

    int formatted_size = vfwslen(format, argv1);
    va_end(argv1);

    wchar_t * formatted = calloc(formatted_size + 1, sizeof(wchar_t));
    vswprintf(formatted, formatted_size, format, argv2);
    va_end(argv2);

    size_t row = 1, row_len = 0;
    wchar_t * cursor = formatted, * end = formatted + formatted_size + 1;
    while (*cursor && cursor < end) {
        con_set_pos((int)(rect.x + row_len), (int)(rect.y + row - 1));
        size_t word = getWordLen(cursor);

        if (word + row_len < rect.width) {  // fits in the row
            wprintf(L"%.*ls", word, cursor);
            if (*(cursor + word) != L'\0') {  // check if we're not skipping the end. if not, safely add a space
                ++word;
                fputws(L" ", stdout);
            }
            row_len += word;
        } else if (row < rect.height) {  // didn't fit, look for a new line
            if (row_len == 0) {  // in case the word doesn't even fit
                wprintf(L"%.*ls", rect.width, cursor);
                cursor += rect.width;
            } else
                wprintf(L"%*hs", rect.width - row_len, "");  // fill rest with bg
            ++row;
            row_len = 0;
            continue;  // new line and try again
        } else {  // if no more new lines = text ellipse time ...
            if (row_len > rect.width - 3)  // no more space remainig
                row_len = rect.width - 3;
            else {
                size_t last = rect.width - row_len;
                wprintf(L"%.*ls", last, cursor);  // write remaining space
                row_len += last;
            }
            row_len += wcslen(cursor);
            row_len = row_len > rect.width - 3 ? rect.width - 3 : row_len - 1;
            con_set_pos((int)(rect.x + row_len), (int)(rect.y + row - 1));
            fputws(L"...", stdout);
            row_len += 3;
            break;
        }

        cursor += word;
    }

    con_set_pos((int)(rect.x + row_len), (int)(rect.y + row - 1));
    wprintf(L"%*hs", rect.width - row_len, "");  // fill rest with bg
    while (row++ < rect.height) {
        con_set_pos((int)rect.x, (int)(rect.y + row - 1));
        wprintf(L"%*hs", rect.width, "");
    }

    free(formatted);

    if (settings->display.colourful)
        con_set_color(COLOR_RESET, COLOR_RESET);
}
