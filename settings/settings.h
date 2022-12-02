#ifndef CHESS_SETTINGS_H
#define CHESS_SETTINGS_H

#include <stdio.h>
#include "../model/defs.h"

typedef enum InputType {
    InputType_WASD = 0,
    InputType_Commands = 1
} InputType;

typedef struct UserSettings {

    struct {
        bool_t colourful;
        bool_t unicode;
    } display;

    InputType inputs;

} UserSettings;

void initDefaultUserSettings(UserSettings * settings);
UserSettings * createDefaultUserSettings();

void saveSettings(UserSettings * settings, FILE * stream);
UserSettings * loadSettings(FILE * stream);

int saveUserSettings(UserSettings * settings);
UserSettings * loadUserSettings();

UserSettings * safeLoadUserSettings();

#endif //CHESS_SETTINGS_H
