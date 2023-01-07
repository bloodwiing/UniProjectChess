#include "menuselector.h"

#include <stdlib.h>

#include "enum/key.h"

#include "ui/con_lib.h"
#include "ui/render.h"

MenuSelector * createMenuSelector(UserSettings * settings, MENU_SELECTOR_INIT_CALLBACK(*init_callback), MENU_SELECTOR_UPDATE_CALLBACK(*update_callback)) {
    MenuSelector * out = calloc(1, sizeof(MenuSelector));
    out->settings = settings;
    out->selected = -1;
    out->init_callback = init_callback;
    out->update_callback = update_callback;
    return out;
}

void addMenuItem(MenuSelector * menu_selector, wchar_t * name, char * text_data, void * other_data, MENU_ITEM_CALLBACK(*callback)) {
    menu_selector->items = realloc(menu_selector->items, sizeof(MenuItem) * ++(menu_selector->item_count));
    menu_selector->items[menu_selector->item_count - 1] = createMenuItem(menu_selector->settings, name, text_data, other_data, callback);
    if (menu_selector->item_count == 1) {
        menu_selector->selected = 0;
        runMenuSelectorUpdateCallback(menu_selector);
    }
}

void displayMenuSelector(MenuSelector * menu_selector, Rect rect) {
    size_t item_count = rect.height < menu_selector->item_count ? rect.height : menu_selector->item_count;
    size_t space = menu_selector->item_count - item_count;
    int offset = (int)(menu_selector->selected - (item_count + 1) / 2 + 1);
    size_t clamped_offset = offset > space ? space : (offset < 0 ? 0 : offset);

    for (size_t i = 0; i < item_count; i++) {
        size_t item_index = i + clamped_offset;

        MenuItem * item = menu_selector->items[item_index];

        int fg = menu_selector->selected == item_index ? COLOR_LIGHT_YELLOW : COLOR_LIGHT_GRAY;
        wchar_t * format = menu_selector->selected == item_index ? L">> %ls" : L"   %ls";

        Rect line = RECT_LINE(rect.x, rect.y + i, rect.width);
#ifdef DEBUG_MENU_RENDERING
        renderTextColouredWrappedRect(menu_selector->settings, COLOR_RED, fg, line, format, item->text, NULL);
#else
        renderTextColouredWrappedRect(menu_selector->settings, COLOR_RESET, fg, line, format, item->text, NULL);
#endif
    }

#ifdef DEBUG_MENU_RENDERING
    con_set_color(COLOR_GREEN, COLOR_BLACK);
    clearRect(offsetRect(rect, 0, (int)item_count, 0, -(int)item_count));
#endif
}

char * getSelectedTextData(MenuSelector * menu_selector) {
    return menu_selector->items[menu_selector->selected]->text_data;
}

void * getSelectedOtherData(MenuSelector * menu_selector) {
    return menu_selector->items[menu_selector->selected]->other_data;
}

void runMenuSelectorInitCallback(MenuSelector * menu_selector) {
    if (menu_selector->init_callback != NULL)
        menu_selector->init_callback(menu_selector->settings);
    runMenuSelectorUpdateCallback(menu_selector);
}

void runMenuSelectorUpdateCallback(MenuSelector * menu_selector) {
    if (menu_selector->update_callback != NULL)
        menu_selector->update_callback(menu_selector->settings, getSelectedTextData(menu_selector), getSelectedOtherData(menu_selector));
}

bool_t updateMenuSelector(MenuSelector * menu_selector, bool_t auto_free) {
    uint32_t key;

    while ((key = con_read_key())) {
        if (key == KEY_ENTER || key == KEY_SPACE) {
            if (!runMenuItem(menu_selector->items[menu_selector->selected])) {
                runMenuSelectorInitCallback(menu_selector);
                continue;
            }
            if (auto_free)
                freeMenuSelector(menu_selector);
            return false;
        }

        switch (key) {
            CASE_KEY_UP:
                if (menu_selector->selected-- == 0) menu_selector->selected = menu_selector->item_count - 1;
                runMenuSelectorUpdateCallback(menu_selector);
                break;
            CASE_KEY_DOWN:
                if (menu_selector->selected++ == menu_selector->item_count - 1) menu_selector->selected = 0;
                runMenuSelectorUpdateCallback(menu_selector);
                break;
            default:
                break;
        }
    }

    if (hasConsoleSizeChanged(menu_selector->settings))
        runMenuSelectorInitCallback(menu_selector);

    return true;
}

void freeMenuSelector(MenuSelector * menu_selector) {
    for (int i = 0; i < menu_selector->item_count;) {
        freeMenuItem(menu_selector->items[i++]);
    }
    free(menu_selector->items);
    free(menu_selector);
}
