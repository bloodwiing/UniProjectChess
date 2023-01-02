#ifndef CHESS_MENUITEM_H
#define CHESS_MENUITEM_H

#include <wchar.h>
#include "../../abstract/defs.h"
#include "../../settings/settings.h"

#define MENU_ITEM_MAX_STRING_LEN 40
#define MENU_ITEM_CALLBACK(name) bool_t (name)(UserSettings * settings, char * data)

typedef struct MenuItem {
    UserSettings * settings;

    wchar_t text[MENU_ITEM_MAX_STRING_LEN];
    char data[MENU_ITEM_MAX_STRING_LEN];
    MENU_ITEM_CALLBACK(*callback);
} MenuItem;

MenuItem * createMenuItem(UserSettings * settings, wchar_t * text, char * data, MENU_ITEM_CALLBACK(*callback));
bool_t runMenuItem(MenuItem * menu_item);

void freeMenuItem(MenuItem * menu_item);

#endif //CHESS_MENUITEM_H
