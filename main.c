#include <stdio.h>
#include "defaults.h"
#include "ui/board.h"
#include "ui/con_lib.h"
#include "settings/settings.h"
#include <locale.h>
#include "utils/files.h"
#include "utils/encoding.h"

void setupConsole();

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    wprintf(L"\nStart with UTF-16\n");
    uint16_t test[] = L"\U0001F601This is a test\U0001F601\U0001F601\u0040";
    wprintf(L"Length U16: %d\n", strU16len(test));
    wprintf(L"Length U16 as U32: %d\n", strU16lenAsU32(test));
    wprintf(L"[");
    for (int i = 0; i <= 21; i++) {
        wprintf(L"%hu ", *(test + i));
    }
    wprintf(L"]\n");

    wprintf(L"\nConvert to UTF-32\n");
    uint32_t * test32 = convertU16toU32(test, 18);
    wprintf(L"Length U32: %d\n", strU32len(test32));
    wprintf(L"Length U32 as U16: %d\n", strU32lenAsU16(test32));
    wprintf(L"[");
    for (int i = 0; i <= 18; i++) {
        wprintf(L"%lu ", *(test32 + i));
    }
    wprintf(L"]\n");

    wprintf(L"\nBack to UTF-16\n");
    uint16_t * res16 = convertU32toU16(test32, 21);
    wprintf(L"Length U16: %d\n", strU16len(res16));
    wprintf(L"Length U16 as U32: %d\n", strU16lenAsU32(res16));
    wprintf(L"[");
    for (int i = 0; i <= 21; i++) {
        wprintf(L"%lu ", *(res16 + i));
    }
    wprintf(L"]\n");

//    UserSettings * settings = safeLoadUserSettings();
//
//    Scenario * scenario = createDefaultScenario();
//    createDirectoryIfMissing("scenario");
//    FILE * file = fopen("scenario/original.chess", "wb");
//    saveScenario(scenario, file);
//    fclose(file);
//
//    con_clear();
//    renderScenario(scenario, settings, 5, 5, 0, 0, 20, 20);
}

#ifdef _WIN32
#include <io.h>
void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);  // _O_U16TEXT
}
#else
void setupConsole() {}
#endif  // _WIN32
