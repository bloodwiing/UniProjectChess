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

    wprintf(L"\nDebug\n");
    uint16_t len_test[] = {10, 55357, 56833, 0};
    wprintf(L"Len test: %d\n", strU16lenAsU32(len_test));

    wprintf(L"\nStart with UTF-16\n");
    uint16_t test[] = {55357, 56833, 84, 104, 105, 115, 32, 105, 115, 32, 97, 32, 116, 101, 115, 116, 55357, 56833, 55357, 56833, 64, 0};
    wprintf(L"Length U16: %d\n", strU16len(test));
    wprintf(L"Length U16 as U32: %d\n", strU16lenAsU32(test));
    wprintf(L"sizeof uint16_t: %d\n", sizeof(uint16_t));
    wprintf(L"sizeof test[]: %d\n", sizeof(test));
    wprintf(L"[");
    for (int i = 0; i <= 21; i++) {
        wprintf(L"%hu ", *(test + i));
    }
    wprintf(L"]\n");

    wprintf(L"\nConvert to UTF-32\n");
    uint32_t * test32 = convertU16toU32(test, 21);
    wprintf(L"Length U32: %d\n", strU32len(test32));
    wprintf(L"Length U32 as U16: %d\n", strU32lenAsU16(test32));
    wprintf(L"[");
    for (int i = 0; i <= 18; i++) {
        wprintf(L"%lu ", *(test32 + i));
    }
    wprintf(L"]\n");

    wprintf(L"\nBack to UTF-16\n");
    uint16_t * res16 = convertU32toU16(test32, 18);
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
