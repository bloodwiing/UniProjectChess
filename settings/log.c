#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "abstract/version.h"

#include "utils/files.h"

#define LOG_FOLDER "./logs"
#define LATEST_FILE "./logs/_latest.log"

#define MAX_TIME_STRING 25

Log * createLog() {
    createDirectoryIfMissing(LOG_FOLDER);

    Log * result = malloc(sizeof(Log));

    char timestamp_file[MAX_TIME_STRING + 5];

    char * time = getCurrentTime(true);
    strcpy(timestamp_file, time);
    free(time);
    strcat(timestamp_file, ".log");
    char * timestamp_file_name = combinePath(LOG_FOLDER, timestamp_file);

    result->timestamp_file = fopen(timestamp_file_name, "w");
    fwide(result->timestamp_file, 1);
    free(timestamp_file_name);

    result->latest = fopen(LATEST_FILE, "w");
    fwide(result->latest, 1);

    fwprintf(result->latest, L"ROOK'S GAMBIT\n");
    fwprintf(result->timestamp_file, L"ROOK'S GAMBIT\n");

    fwprintf(result->latest, L"Version: %hs (%d)\n", getVersionName(BUILD_VERSION), BUILD_VERSION);
    fwprintf(result->timestamp_file, L"Version: %hs (%d)\n", getVersionName(BUILD_VERSION), BUILD_VERSION);

    return result;
}

char * getCurrentTime(bool_t file_safe) {
    char * result = calloc(MAX_TIME_STRING, sizeof(char));

    time_t raw_time;
    struct tm * time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    if (file_safe)
        strftime(result, MAX_TIME_STRING, "%Y-%m-%d %H-%M-%S", time_info);
    else
        strftime(result, MAX_TIME_STRING, "%Y-%m-%d %X", time_info);

    return result;
}

void printCurrentTime(FILE * stream) {
    char * time = getCurrentTime(false);
    fwprintf(stream, L"%hs ", time);
    free(time);
}

void writeLogEntryStream(FILE * file, const wchar_t * module, const wchar_t * format, const wchar_t * suffix, va_list va) {
    printCurrentTime(file);
    if (suffix != NULL)
        fputws(suffix, file);
    fputws(module, file);
    fputws(L": ", file);
    vfwprintf(file, format, va);
    fputws(L"\n", file);
    fflush(file);
}

void writeLogEntry(Log * log, const wchar_t * module, const wchar_t * format, const wchar_t * suffix, va_list va) {
    va_list va_2;
    va_copy(va_2, va);

    writeLogEntryStream(log->latest, module, format, suffix, va_2);
    writeLogEntryStream(log->timestamp_file, module, format, suffix, va_2);
}

void v_logDebug(Log * log, const wchar_t * module, const wchar_t * format, va_list va) {
    writeLogEntry(log, module, format, L"[ DBG ] ", va);
}

void v_logInfo(Log * log, const wchar_t * module, const wchar_t * format, va_list va) {
    writeLogEntry(log, module, format, L"[ INF ] ", va);
}

void v_logWarning(Log * log, const wchar_t * module, const wchar_t * format, va_list va) {
    writeLogEntry(log, module, format, L"[ WRN ] ", va);
}

void v_logError(Log * log, const wchar_t * module, const wchar_t * format, va_list va) {
    writeLogEntry(log, module, format, L"[ ERR ] ", va);
}

void freeLog(Log * log) {
    fwprintf(log->latest, L"Program successfully shut down\n");
    fwprintf(log->timestamp_file, L"Program successfully shut down\n");
    fclose(log->latest);
    fclose(log->timestamp_file);
    free(log);
}
