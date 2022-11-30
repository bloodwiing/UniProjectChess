#include <stdio.h>
#include "defaults.h"
#include "ui/board.h"
#include "ui/con_lib.h"
#include "settings/settings.h"

int main() {
    UserSettings * settings = safeLoadUserSettings();
    printf("%d %d %d", settings->inputs, settings->display.unicode, settings->display.colorful);
}
