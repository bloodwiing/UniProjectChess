#include "promotionmenu.h"

#include <string.h>

#include "enum/key.h"

#include "ui/component/dialogbox.h"
#include "ui/render.h"

typedef struct PromotionMenuDialogData {
    Team * team;
    piece_index_t selected;
} PromotionMenuDialogData;

DIALOGBOX_CONTENT_CALLBACK(dialogBoxContent);

piece_index_t promotionMenuLoop(UserSettings * settings, Team * team) {
    ditherEffect();

    piece_index_t selected = 0;
    bool_t confirmed = false;

    PromotionMenuDialogData data = {.team = team, .selected = team->promotions[0]};

    DialogBox box = createDialogBox(L"CHOOSE YOUR PROMOTION", 4, team->promotion_count * PIECE_UNICODE_LENGTH, dialogBoxContent);
    box.data_ptr = &data;

    drawDialogBoxCentered(settings, &box);

    while (!confirmed) {
        key_code_t key;
        while ((key = con_read_key())) {
            switch (key) {
                CASE_KEY_LEFT:
                CASE_KEY_DOWN:
                    if (selected-- == 0) selected = team->promotion_count - 1;
                    break;

                CASE_KEY_RIGHT:
                CASE_KEY_UP:
                    if (++selected == team->promotion_count) selected = 0;
                    break;

                CASE_KEY_CONFIRM:
                    confirmed = true;
                    break;

                default:
                    break;
            }

            data.selected = team->promotions[selected];
            runDialogBoxCallback(settings, &box);
        }
    }

    con_clear();
    return team->promotions[selected];
}

DIALOGBOX_CONTENT_CALLBACK(dialogBoxContent) {
    PromotionMenuDialogData * data = data_ptr;

    // choices
    Rect choices_rect = centerRectInRect(data->team->promotion_count * 2 - 1, 1, createRect(x, y, width, height));

    for (piece_index_t i = 0; i < data->team->promotion_count; i++) {
        con_set_pos(choices_rect.x + 2 * i, y);

        piece_index_t promotion = data->team->promotions[i];
        int colour = promotion == data->selected ? COLOR_LIGHT_GREEN : COLOR_LIGHT_GRAY;
        
        renderPieceColoured(settings, COLOR_RESET, colour, data->team->pieces[data->team->promotions[i]]);
    }

    // selected piece name
    Piece selected = data->team->pieces[data->selected];
    Rect selected_rect = centerRectInRect((int)strlen(selected.name), 1, createRect(x, y + 1, width, height));

    con_set_pos(x, selected_rect.y);
    wprintf(L"%*s", width, "");
    con_set_pos(selected_rect.x, selected_rect.y);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GREEN, L"%hs", selected.name);
}
