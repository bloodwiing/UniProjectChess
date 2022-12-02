#include "settings.h"
#include <stdlib.h>
#include "../utils/files.h"

const char user_setting_string[] = "# ROOK'S GAMBIT Settings\n"
                                   "input=%d\n"
                                   "colourful=%d\n"
                                   "unicode=%d";

void initDefaultUserSettings(UserSettings * settings) {
    settings->inputs = InputType_WASD;
    settings->display.colourful = true;
    settings->display.unicode = false;
}

UserSettings * createDefaultUserSettings() {
    UserSettings * out = malloc(sizeof(UserSettings));
    initDefaultUserSettings(out);
    return out;
}

void saveSettings(UserSettings * settings, FILE * stream) {
    fprintf(stream, user_setting_string, settings->inputs, settings->display.colourful, settings->display.unicode);
}

UserSettings * loadSettings(FILE * stream) {
    UserSettings * out = calloc(sizeof(UserSettings), 1);
    fscanf(stream, user_setting_string, (int *)&out->inputs, (int *)&out->display.colourful, (int *)&out->display.unicode);
    return out;
}

int saveUserSettings(UserSettings * settings) {
    createDirectoryIfMissing("data");
    FILE * file = fopen("data/settings.ini", "wb");
    if (file == NULL) return -1;
    saveSettings(settings, file);
    fclose(file);
    return 0;
}

UserSettings * loadUserSettings() {
    FILE * file = fopen("data/settings.ini", "rb");
    if (file == NULL) return NULL;
    UserSettings * settings = loadSettings(file);
    fclose(file);
    return settings;
}

UserSettings * safeLoadUserSettings() {
    UserSettings * settings = loadUserSettings();
    if (settings == NULL) {
        settings = createDefaultUserSettings();
        saveUserSettings(settings);
    }
    return settings;
}
