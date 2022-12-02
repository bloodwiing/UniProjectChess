#include "menuitem.h"
#include <stdlib.h>

MenuItem * createMenuItem(wchar_t * text, void (*callback)()) {
    MenuItem * out = malloc(sizeof(MenuItem));
    wcscpy(out->text, text);
    out->callback = callback;
    return out;
}

void runMenuItem(MenuItem * menu_item) {
    menu_item->callback();
}
