#ifndef CHESS_MENUITEM_H
#define CHESS_MENUITEM_H

#include <wchar.h>
#include "../../abstract/defs.h"
#include "../../settings/settings.h"

#define MENU_ITEM_MAX_STRING_LEN 40

typedef struct MenuItem {
    UserSettings * settings;

    wchar_t text[MENU_ITEM_MAX_STRING_LEN];
    char data[MENU_ITEM_MAX_STRING_LEN];
    void (*callback)(UserSettings *, char *);
} MenuItem;

MenuItem * createMenuItem(UserSettings * settings, wchar_t * text, char * data, void (*callback)(UserSettings *, char *));
void runMenuItem(MenuItem * menu_item);

#endif //CHESS_MENUITEM_H
