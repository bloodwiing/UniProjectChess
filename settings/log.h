#ifndef CHESS_LOG_H
#define CHESS_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#include "abstract/defs.h"

typedef struct Log {
    FILE * latest;
    FILE * timestamp_file;
} Log;

Log * createLog();

char * getCurrentTime(bool_t file_safe);
void printCurrentTime(FILE * stream);

void v_logDebug(Log * log, const wchar_t * module, const wchar_t * format, va_list va);
void v_logInfo(Log * log, const wchar_t * module, const wchar_t * format, va_list va);
void v_logWarning(Log * log, const wchar_t * module, const wchar_t * format, va_list va);
void v_logError(Log * log, const wchar_t * module, const wchar_t * format, va_list va);

void freeLog(Log * log);

#endif //CHESS_LOG_H
