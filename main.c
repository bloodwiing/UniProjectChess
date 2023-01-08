#include <stdio.h>
#include <locale.h>

#include "defaults.h"

#include "settings/settings.h"

#include "ui/con_lib.h"
#include "ui/view/mainmenu.h"

void setupConsole();

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    con_show_echo(false);
    con_show_cursor(false);

    logInfo(settings, L"Main", L"Saving default scenario");
    saveDefaultScenario(settings);

    mainMenuLoop(settings);
    con_clear();

    wprintf(L"Saving log file...\n");
    freeSettings(settings);

    con_show_echo(true);
    con_show_cursor(true);
}

#ifdef _WIN32
#include <io.h>

void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);
}
#else
void setupConsole() {}
#endif  // _WIN32
