#include <stdio.h>
#include "defaults.h"
#include "ui/con_lib.h"
#include "settings/settings.h"
#include <locale.h>
#include "ui/view/mainmenu.h"
#include "ui/view/gamemenu.h"
#include "ui/render.h"
#include "engine/validation.h"
#include <string.h>

void setupConsole();

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    con_show_echo(false);
    con_show_cursor(false);

//    wchar_t * original = L"♝";
//    wchar_t * u32cursor = original;
//    while (*u32cursor) {
//        wprintf(L"%ld ", (uint32_t) *u32cursor++);
//    }
//    wprintf(L"\n%ls\n", original);
//
//    size_t original_size = strU32len((uint32_t *)original);
//    size_t convert_size = strU32lenAsU16((uint32_t *)original);
//
//    wchar16_t * converted = createU16(L"♝", PIECE_UNICODE_LENGTH);
//    wchar16_t * u16cursor = converted;
//    while (*u16cursor) {
//        wprintf(L"%d ", *u16cursor++);
//    }
//    wprintf(L"\n%ls\n", converted);
//
//    size_t convert_back_size = strU16lenAsU32(converted);
//
//    wchar_t * returned = createWStr(converted, PIECE_UNICODE_LENGTH);
//    u32cursor = returned;
//    while (*u32cursor) {
//        wprintf(L"%ld ", (uint32_t) *u32cursor++);
//    }
//    wprintf(L"\n%ls\n", original);
//
//    free(converted);
//    free(returned);

    FILE * f = fopen("./scenario/classic.chess", "wb");
    Scenario * s = createDefaultScenario();
    saveScenario(s, f, true);
    fclose(f);
    freeScenario(s);
//
//
//    Piece * pieces = calloc(2, sizeof(Piece));
//    initDefaultKnight(pieces + 0, 0);
//    initDefaultKing(pieces + 1, 0);
//
//    Team * team1 = createTeam("Black", COLOR_RED, pieces, 2, TeamDirectionUp);
//
//    Piece * pieces2 = calloc(2, sizeof(Piece));
//    initDefaultKnight(pieces2 + 0, 1);
//    initDefaultKing(pieces2 + 1, 1);
//
//    Team * team2 = createTeam("White", COLOR_LIGHT_YELLOW, pieces2, 2, TeamDirectionDown);
//
//    Team * teams = calloc(2, sizeof(Team));
//    memcpy(teams + 0, team1, sizeof(Team));
//    memcpy(teams + 1, team2, sizeof(Team));
//
//    Spawn * spawns = calloc(16, sizeof(Spawn));
//    spawns[0] = *createSpawn(0, 0, 0, 0);  // black knight
//    spawns[1] = *createSpawn(1, 0, 0, 0);  // black knight
//    spawns[2] = *createSpawn(2, 0, 0, 1);  // black king
//    spawns[3] = *createSpawn(3, 0, 0, 0);  // black knight
//    spawns[4] = *createSpawn(4, 0, 0, 0);  // black knight
//    spawns[5] = *createSpawn(1, 1, 0, 0);  // black knight
//    spawns[6] = *createSpawn(2, 1, 0, 0);  // black knight
//    spawns[7] = *createSpawn(3, 1, 0, 0);  // black knight
//
//    spawns[8] = *createSpawn(0, 4, 1, 0);  // white knight
//    spawns[9] = *createSpawn(1, 4, 1, 0);  // white knight
//    spawns[10] = *createSpawn(2, 4, 1, 1);  // white king
//    spawns[11] = *createSpawn(3, 4, 1, 0);  // white knight
//    spawns[12] = *createSpawn(4, 4, 1, 0);  // white knight
//    spawns[13] = *createSpawn(1, 3, 1, 0);  // white knight
//    spawns[14] = *createSpawn(2, 3, 1, 0);  // white knight
//    spawns[15] = *createSpawn(3, 3, 1, 0);  // white knight
//
//    Scenario * scenario = createScenario("Knight Square", "Other BLOODWIING", 5, 5, teams, 2, spawns, 16);
//    FILE * file = fopen("scenario/knights.chess", "wb");
//    saveScenario(scenario, file);
//    fclose(file);


    mainMenuLoop(settings);
//    Scenario * scenario = createDefaultScenario();
//    Exception exception;
//    Board * board = createBoard(scenario, settings, &exception);
//    if (board == NULL && exception.status) {
//        reportException(exception);
//        freeScenario(scenario);
//    } else {
//        FILE * file = fopen("./data/save.bin", "wb");
//        saveBoard(board, file);
//        fclose(file);
//        freeBoard(board, true);
//    }
//    Exception exception;
//    GameState * state = loadGameStateDefault(settings, &exception);
//    if (state == NULL && exception.status) {
//        reportException(exception);
//    }
//    isMoveValid(state->board, 3, 6, 3, 5);
//    freeBoard(state->board, true);
//    free(state);

//    Scenario * scenario = createDefaultScenario();
//
//    renderScenario(scenario, settings, 50, 2, 0, 0, 30, 10);
//
//    con_set_pos(50, 14);
//    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GREEN, L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->name);
//    con_set_pos(50, 15);
//    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Author: ");
//    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->author);
//    con_set_pos(50, 17);
//    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Size: ");
//    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%d x %d", scenario->size_x, scenario->size_y);
//    con_set_pos(50, 18);
//    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Teams: ");
//    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%d", scenario->team_count);

    freeSettings(settings);
}

#ifdef _WIN32
#include <io.h>

void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);
}
#else
void setupConsole() {}
#endif  // _WIN32
