#include "exception.h"
#include "../ui/con_lib.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

void printExceptionLineWithColour(wchar_t * type, wchar_t * format, ...) {
    con_set_color(COLOR_RESET, COLOR_RED);
    wprintf(type);
    con_set_color(COLOR_RESET, COLOR_WHITE);
    va_list va;
    va_start(va, format);
    vwprintf(format, va);
    va_end(va);
}

void reportExceptionAtPos(Exception exception, int x, int y) {
    con_set_color(COLOR_RESET, COLOR_LIGHT_RED);

    con_set_pos(x++, y);
    wprintf(L"[ ERROR ]\n");

    con_set_pos(x, y + 1);
    printExceptionLineWithColour(L"Status: ", L"%d (0x%X)", exception.status, exception.status);

    con_set_pos(x, y + 2);
    printExceptionLineWithColour(L"Fatal: ", L"%ls", exception.fatal ? L"Yes" : L"No");

    con_set_pos(x, y + 3);
    printExceptionLineWithColour(L"Message: ", L"%hs\n", exception.message);

    con_set_color(COLOR_RESET, COLOR_RESET);
}

void reportException(Exception exception) {
    con_clear();
    reportExceptionAtPos(exception, 2, 2);
    con_flush();
    con_sleep(10);
    con_clear();
}
