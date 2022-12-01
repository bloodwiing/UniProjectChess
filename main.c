#include <stdio.h>
#include "defaults.h"
#include "ui/board.h"
#include "ui/con_lib.h"
#include "settings/settings.h"
#include <locale.h>
#include "utils/files.h"

void setupConsole();

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    createDirectoryIfMissing("scenario");
//    FILE * file = fopen("scenario/original.chess", "rb");
//    Scenario * scenario = loadScenario(file);
//    fclose(file);
    FILE * file = fopen("scenario/original.chess", "wb");
    Scenario * scenario = createDefaultScenario();
    saveScenario(scenario, file);
    fclose(file);

    wprintf(L"%d\n", sizeof(void *));

//    con_clear();
//    renderScenario(scenario, settings, 5, 5, 0, 0, 20, 20);
}

#ifdef _WIN32
#include <io.h>

void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);
}
#else
void setupConsole() {}
#endif  // _WIN32
