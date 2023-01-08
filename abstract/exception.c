#include "exception.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "ui/con_lib.h"
#include "ui/generic/text.h"

void clearException(Exception * exception) {
    exception->status = 0;
    exception->fatal = false;
    exception->message[0] = 0;
}

void updateException(Exception * exception, int status, bool_t fatal, char * message) {
    exception->status = status;
    exception->fatal = fatal;
    strncpy(exception->message, message, EXCEPTION_MESSAGE_LENGTH);
}


void reportExceptionAtPos(UserSettings * settings, Exception exception, Rect rect) {
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_RED, RECT_LINE(rect.x++, rect.y, rect.width), L"[ ERROR ]");

    con_set_pos(rect.x, ++rect.y);
    renderTextColoured(settings, COLOR_RESET, COLOR_RED, L"Status: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_WHITE, RECT_LINE(rect.x + 8, rect.y, rect.width - 8), L"%d (0x%X)", exception.status, exception.status);

    con_set_pos(rect.x, ++rect.y);
    renderTextColoured(settings, COLOR_RESET, COLOR_RED, L"Fatal: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_WHITE, RECT_LINE(rect.x + 7, rect.y, rect.width - 7), L"%ls", exception.fatal ? L"Yes" : L"No");

    con_set_pos(rect.x, ++rect.y);
    renderTextColoured(settings, COLOR_RESET, COLOR_RED, L"Message: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_WHITE, offsetRect(rect, 9, 0, -9, -3), L"%hs", exception.message);

    con_set_color(COLOR_RESET, COLOR_RESET);
}

void reportException(UserSettings * settings, Exception exception) {
    con_clear();
    reportExceptionAtPos(settings, exception, offsetRect(getScreenRect(), 2, 2, -4, -4));
    con_flush();
    con_sleep(5.0f);
    con_clear();
}
