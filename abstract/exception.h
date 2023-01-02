#ifndef CHESS_EXCEPTION_H
#define CHESS_EXCEPTION_H

#include "defs.h"

#define EXCEPTION_MESSAGE_LENGTH 50

typedef struct Exception {
    int status;
    bool_t fatal;

    char message[EXCEPTION_MESSAGE_LENGTH];
} Exception;

void updateException(Exception * exception, int status, bool_t fatal, char * message);
void reportExceptionAtPos(Exception exception, int x, int y);
void reportException(Exception exception);

#endif //CHESS_EXCEPTION_H
