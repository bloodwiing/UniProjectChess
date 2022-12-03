#include "menuitem.h"
#include <stdlib.h>
#include <string.h>

MenuItem * createMenuItem(UserSettings * settings, wchar_t * text, char * data, void (*callback)(UserSettings *, char *)) {
    MenuItem * out = malloc(sizeof(MenuItem));
    out->settings = settings;
    wcsncpy(out->text, text, MENU_ITEM_MAX_STRING_LEN);
    strncpy(out->data, data, MENU_ITEM_MAX_STRING_LEN);
    out->callback = callback;
    return out;
}

void runMenuItem(MenuItem * menu_item) {
    menu_item->callback(menu_item->settings, menu_item->data);
}
