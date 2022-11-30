#include <stdio.h>
#include "defaults.h"
#include "ui/board.h"
#include "ui/con_lib.h"
#include "settings/settings.h"
#include <locale.h>
#include <io.h>

int main() {
    setlocale(LC_ALL, "");
#ifdef _WIN32
    _setmode(_fileno(stdout), 0x00040000);  // _O_U16TEXT
#endif

    UserSettings * settings = safeLoadUserSettings();

    Scenario * scenario = createDefaultScenario();
    FILE * file = fopen("scenario/original.chess", "wb");
    saveScenario(scenario, file);
    fclose(file);

    con_clear();
    renderScenario(scenario, settings, 5, 5, 0, 0, 20, 20);
}
