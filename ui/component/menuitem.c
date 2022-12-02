#include "menuitem.h"
#include <stdlib.h>

MenuItem * createMenuItem(wchar_t * text, void (*callback)()) {
    MenuItem * out = malloc(sizeof(MenuItem));
    wcscpy(out->text, text);
    out->callback = callback;
    out->selected = false;
    return out;
}

void run(MenuItem * menu_item) {
    menu_item->callback();
}
