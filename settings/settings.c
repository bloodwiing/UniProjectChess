#include "settings.h"

#include <stdlib.h>

#include "utils/files.h"

#define SETTINGS_FILE "./data/settings.ini"

const char user_setting_string[] = \
"# ROOK'S GAMBIT Settings\n"
"[DISPLAY]\n"
"colourful=%d\n"
"unicode=%d\n"
"\n"
"[INPUT]\n"
"input=%d\n"
"\n"
"[HISTORY]\n"
"notation=%d\n"
"figurine=%d";

#define SETTINGS_SCAN_CHECK 5

void initDefaultUserSettings(UserSettings * settings) {
    settings->inputs = InputType_WASD;
    settings->display.colourful = true;
    settings->display.unicode = false;
    settings->notation = NotationType_SHORT;
    settings->notation_figurines = false;
}

UserSettings * createDefaultUserSettings() {
    UserSettings * out = malloc(sizeof(UserSettings));
    initDefaultUserSettings(out);
    out->size = con_get_size();
    return out;
}

void saveSettings(UserSettings * settings, FILE * stream) {
    fprintf(stream, user_setting_string, settings->display.colourful, settings->display.unicode, settings->inputs, settings->notation, settings->notation_figurines);
}

UserSettings * loadSettings(FILE * stream) {
    UserSettings * out = calloc(1, sizeof(UserSettings));
    int input_type;
    int notation_type;
    if (fscanf(stream, user_setting_string, (int *)&out->display.colourful, (int *)&out->display.unicode, &input_type, &notation_type, (int *)&out->notation_figurines) != SETTINGS_SCAN_CHECK) {
        free(out);
        return createDefaultUserSettings();
    }
    out->inputs = input_type;
    out->notation = notation_type;
    out->size = con_get_size();
    return out;
}

int saveUserSettings(UserSettings * settings) {
    createDirectoryIfMissing("data");
    FILE * file = fopen(SETTINGS_FILE, "wb");
    if (file == NULL) return -1;
    saveSettings(settings, file);
    fclose(file);
    return 0;
}

UserSettings * loadUserSettings() {
    FILE * file = fopen(SETTINGS_FILE, "rb");
    if (file == NULL)
        return NULL;
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

bool_t hasConsoleSizeChanged(UserSettings * settings) {
    ConSize new = con_get_size();
    if (new.width != settings->size.width || new.height != settings->size.height) {
        settings->size = new;
        return true;
    }
    return false;
}

void freeSettings(UserSettings * settings) {
    free(settings);
}
