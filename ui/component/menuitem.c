#include "menuitem.h"

#include <stdlib.h>
#include <string.h>

MenuItem * createMenuItem(UserSettings * settings, wchar_t * text, char * text_data, void * other_data, MENU_ITEM_CALLBACK(*callback)) {
    MenuItem * out = malloc(sizeof(MenuItem));
    out->settings = settings;
    wcsncpy(out->text, text, MENU_ITEM_MAX_STRING_LEN);
    strncpy(out->text_data, text_data, MENU_ITEM_MAX_STRING_LEN);
    out->other_data = other_data;
    out->callback = callback;
    return out;
}

bool_t runMenuItem(MenuItem * menu_item) {
    return menu_item->callback(menu_item->settings, menu_item->text_data);
}

void freeMenuItem(MenuItem * menu_item) {
    free(menu_item);
}
