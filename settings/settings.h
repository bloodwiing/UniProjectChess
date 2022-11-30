#ifndef CHESS_SETTINGS_H
#define CHESS_SETTINGS_H

#include <stdio.h>
#include "../model/defs.h"

typedef enum InputType {
    WASD = 0,
    Arrows = 1,
    Commands = 2
} InputType;

typedef struct UserSettings {

    struct {
        bool colorful;
        bool unicode;
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
