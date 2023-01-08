#include <stdio.h>
#include <locale.h>
#include <sys/time.h>
#include <string.h>

#include "./test.h"

#include "defaults.h"

#include "settings/settings.h"

#include "ui/con_lib.h"
#include "ui/view/mainmenu.h"

void setupConsole();

int main(int argc, char ** argv) {
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        runTests();
        return 0;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    con_show_echo(false);
    con_show_cursor(false);

    logInfo(settings, L"Main", L"Saving default scenario");
    saveDefaultScenario(settings);

    mainMenuLoop(settings);
    con_set_color(COLOR_RESET, COLOR_RESET);
    con_clear();

    gettimeofday(&end, NULL);
    uint64_t delta_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

    uint64_t final_s = delta_ms / 1000;
    uint64_t final_ms = delta_ms % 1000;

    wprintf(L"Program finished in %ds %dms\n", final_s, final_ms);
    logInfo(settings, L"Main", L"Program finished in %ds %dms\n", final_s, final_ms);

    wprintf(L"Saving log file...\n");
    freeSettings(settings);

    wprintf(L"Log saved to %hs\n", LATEST_FILE);

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
