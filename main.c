#include <stdio.h>
#include "defaults.h"
#include "ui/board.h"
#include "ui/con_lib.h"
#include "settings/settings.h"
#include <locale.h>
#include "utils/files.h"
#include "ui/view/mainmenu.h"
#include "ui/view/scenariomenu.h"
#include "ui/render.h"
#include "defaults.h"

void setupConsole();

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    con_show_echo(false);
    con_show_cursor(false);

//    createDirectoryIfMissing("scenario");
//    FILE * f = fopen("scenario/original.chess", "rb");
//    Scenario * s = loadScenario(f);
//    fclose(f);
    FILE * f = fopen("scenario/original.chess", "wb");
    Scenario * s = createDefaultScenario();
    saveScenario(s, f);
    fclose(f);

//    wprintf(L"%d\n", sizeof(void *));

    con_clear();
//    renderScenario(scenario, settings, 5, 5, 0, 0, 20, 20);

    Piece * pieces = calloc(2, sizeof(Piece));
    initDefaultKnight(pieces + 0, 0);
    initDefaultKing(pieces + 1, 0);

    Team * team1 = createTeam("Black", COLOR_RED, pieces, 2);

    Piece * pieces2 = calloc(2, sizeof(Piece));
    initDefaultKnight(pieces2 + 0, 1);
    initDefaultKing(pieces2 + 1, 1);

    Team * team2 = createTeam("White", COLOR_LIGHT_YELLOW, pieces2, 2);

    Team * teams = calloc(2, sizeof(Team));
    memcpy(teams + 0, team1, sizeof(Team));
    memcpy(teams + 1, team2, sizeof(Team));

    Spawn * spawns = calloc(16, sizeof(Spawn));
    spawns[0] = *createSpawn(0, 0, 0, 0);  // black knight
    spawns[1] = *createSpawn(1, 0, 0, 0);  // black knight
    spawns[2] = *createSpawn(2, 0, 0, 1);  // black king
    spawns[3] = *createSpawn(3, 0, 0, 0);  // black knight
    spawns[4] = *createSpawn(4, 0, 0, 0);  // black knight
    spawns[5] = *createSpawn(1, 1, 0, 0);  // black knight
    spawns[6] = *createSpawn(2, 1, 0, 0);  // black knight
    spawns[7] = *createSpawn(3, 1, 0, 0);  // black knight

    spawns[8] = *createSpawn(0, 4, 1, 0);  // white knight
    spawns[9] = *createSpawn(1, 4, 1, 0);  // white knight
    spawns[10] = *createSpawn(2, 4, 1, 1);  // white king
    spawns[11] = *createSpawn(3, 4, 1, 0);  // white knight
    spawns[12] = *createSpawn(4, 4, 1, 0);  // white knight
    spawns[13] = *createSpawn(1, 3, 1, 0);  // white knight
    spawns[14] = *createSpawn(2, 3, 1, 0);  // white knight
    spawns[15] = *createSpawn(3, 3, 1, 0);  // white knight

    Scenario * scenario = createScenario("Lol", 5, 5, teams, 2, spawns, 16);
    FILE * file = fopen("scenario/lol.chess", "wb");
    saveScenario(scenario, file);
    fclose(file);

    mainMenuLoop(settings);
//    scenarioMenuLoop(settings);
}

#ifdef _WIN32
#include <io.h>

void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);
}
#else
void setupConsole() {}
#endif  // _WIN32
