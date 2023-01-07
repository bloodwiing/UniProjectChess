#ifndef CHESS_EXCEPTION_H
#define CHESS_EXCEPTION_H

#include "defs.h"

#include "ui/generic/rect.h"

#include "settings/settings.h"

#define EXCEPTION_MESSAGE_LENGTH 100

typedef struct Exception {
    int status;
    bool_t fatal;

    char message[EXCEPTION_MESSAGE_LENGTH];
} Exception;

void clearException(Exception * exception);

void updateException(Exception * exception, int status, bool_t fatal, char * message);
void reportExceptionAtPos(UserSettings * settings, Exception exception, Rect rect);
void reportException(UserSettings * settings, Exception exception);

#endif //CHESS_EXCEPTION_H
