#ifndef CHESS_SETTINGS_H
#define CHESS_SETTINGS_H

#include <stdio.h>

#include "abstract/defs.h"

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

} UserSettings;

void initDefaultUserSettings(UserSettings * settings);
UserSettings * createDefaultUserSettings();

void saveSettings(UserSettings * settings, FILE * stream);
UserSettings * loadSettings(FILE * stream);

int saveUserSettings(UserSettings * settings);
UserSettings * loadUserSettings();

UserSettings * safeLoadUserSettings();

void freeSettings(UserSettings * settings);

#endif //CHESS_SETTINGS_H
