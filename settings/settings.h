#ifndef CHESS_SETTINGS_H
#define CHESS_SETTINGS_H

#include <stdarg.h>
#include <stdio.h>

#include "./log.h"

#include "abstract/defs.h"

#include "ui/con_lib.h"

typedef enum InputType {
    InputType_WASD = 0,
    InputType_Commands = 1
} InputType;

typedef enum NotationType {
    NotationType_SHORT = 0,
    NotationType_LONG = 1
} NotationType;

typedef struct UserSettings {

    struct {
        bool_t colourful;
        bool_t unicode;
    } display;

    InputType inputs;

    NotationType notation;
    bool_t notation_figurines;

    // runtime data
    ConSize size;
    Log * _log;
} UserSettings;

void initDefaultUserSettings(UserSettings * settings);
UserSettings * createDefaultUserSettings();

void saveSettings(UserSettings * settings, FILE * stream);
UserSettings * loadSettings(FILE * stream);

int saveUserSettings(UserSettings * settings);
UserSettings * loadUserSettings();

UserSettings * safeLoadUserSettings();

bool_t hasConsoleSizeChanged(UserSettings * settings);

void logDebug(UserSettings * settings, const wchar_t * module, const wchar_t * format, ...);
void logInfo(UserSettings * settings, const wchar_t * module, const wchar_t * format, ...);
void logWarning(UserSettings * settings, const wchar_t * module, const wchar_t * format, ...);
void logError(UserSettings * settings, const wchar_t * module, const wchar_t * format, ...);

void freeSettings(UserSettings * settings);

#endif //CHESS_SETTINGS_H
