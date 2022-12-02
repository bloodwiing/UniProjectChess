#ifndef CHESS_MENUITEM_H
#define CHESS_MENUITEM_H

#include <wchar.h>
#include "../../model/defs.h"
#include "../../settings/settings.h"

#define MenuItemMaxStringLen 20

typedef struct MenuItem {
    UserSettings * settings;

    wchar_t text[MenuItemMaxStringLen];
    char data[MenuItemMaxStringLen];
    void (*callback)(UserSettings *, char *);
} MenuItem;

MenuItem * createMenuItem(UserSettings * settings, wchar_t * text, char * data, void (*callback)(UserSettings *, char *));
void runMenuItem(MenuItem * menu_item);

#endif //CHESS_MENUITEM_H
