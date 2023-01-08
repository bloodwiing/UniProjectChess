#include "dialogbox.h"

#include <stdlib.h>
#include <string.h>

#include "ui/con_lib.h"
#include "ui/render.h"

#define LOG_MODULE L"DialogBox"

#define COLOUR_SKULL COLOR_WHITE
#define COLOUR_ROSE COLOR_LIGHT_RED
#define COLOUR_ROSE_STEM COLOR_GREEN
#define COLOUR_BORDER COLOR_DARK_GRAY
#define COLOUR_TITLE COLOR_LIGHT_YELLOW

DialogBox createDialogBox(wchar_t * title, uint16_t height, uint16_t width, DIALOGBOX_CONTENT_CALLBACK(*content_callback)) {
    DialogBox out = {.height = height, .width = width, .content_callback = content_callback};
    wcsncpy(out.title, title, DIALOGBOX_MAX_TITLE_LEN);

    if (width < 9) out.width = 9;
    if (height < 3) out.height = 3;

    size_t title_len = wcslen(title);
    if (title_len > width) out.width = title_len;

    return out;
}

void runDialogBoxCallback(UserSettings * settings, DialogBox * dialog_box) {
    con_set_pos(dialog_box->x + 9, dialog_box->y + 5);
    dialog_box->content_callback(settings, dialog_box->x + 9, dialog_box->y + 5, dialog_box->width, dialog_box->height - 2, dialog_box->data_ptr);
    con_flush();
}

void drawDialogBox(UserSettings * settings, DialogBox * dialog_box, int x, int y) {
    logInfo(settings, LOG_MODULE, L"Rendering Dialog box");
    logDebug(settings, LOG_MODULE, L"width=%d height=%d", dialog_box->width, dialog_box->height);

    con_set_color(COLOR_RESET, COLOR_RESET);

    dialog_box->x = x;
    dialog_box->y = y;

    int title_pad = (int)(dialog_box->width - wcslen(dialog_box->title));
    int title_pad_left = title_pad / 2;

    logDebug(settings, LOG_MODULE, L"title_pad=%d title_pad_left=%d", title_pad, title_pad_left);

    int border_pad = dialog_box->width - 9;
    int border_pad_left = border_pad / 2;

    logDebug(settings, LOG_MODULE, L"border_pad=%d border_pad_left=%d", border_pad, border_pad_left);

    wchar_t * border_left = calloc(border_pad_left + 1, sizeof(wchar_t));
    wmemset(border_left, L'─', border_pad_left);

    wchar_t * border_right = calloc(border_pad - border_pad_left + 1, sizeof(wchar_t));
    wmemset(border_right, L'─', border_pad - border_pad_left);

    con_set_pos(x, y);
    renderTextColoured(settings, COLOR_RESET, COLOUR_SKULL, L"%*hs            .-.            %*hs", border_pad_left, "", border_pad - border_pad_left, "");
    con_set_pos(x, y + 1);
    renderTextColoured(settings, COLOR_RESET, COLOUR_SKULL, L"%*hs           ('.')           %*hs", border_pad_left, "", border_pad - border_pad_left, "");
    con_set_pos(x, y + 2);
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L" _ ");
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE, L"Y  ");
    renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"╓─%ls─", border_left);
    renderTextColoured(settings, COLOR_RESET, COLOUR_SKULL, L"'=.|m|.='");
    renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"─%ls─╖", border_right);
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE, L"  Y");
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L" _ ");
    con_set_pos(x, y + 3);
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE, L">");
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L" )|'_");
    renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"║  ");
    renderTextColoured(settings, COLOR_RESET, COLOUR_TITLE, L"%*hs%ls%*hs", title_pad_left, "", dialog_box->title, title_pad - title_pad_left, "");
    renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"  ║");
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L"_`|( ");
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE, L"<");
    con_set_pos(x, y + 4);
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L"  \\(/ ");
    renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"║  %*hs  ║", dialog_box->width, "");
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L" \\)/  ");
    con_set_pos(x, y + 5);
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L"   `\\ ");
    renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"║  %*hs  ║", dialog_box->width, "");
    renderTextColoured(settings, COLOR_RESET, COLOUR_ROSE_STEM, L" /'   ");

    for (int i = 3; i < dialog_box->height;) {
        con_set_pos(x, y + 3 + i++);
        renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"      ║  %*hs  ║      ", dialog_box->width, "");
    }

    con_set_pos(x, y + 3 + dialog_box->height);
    renderTextColoured(settings, COLOR_RESET, COLOUR_BORDER, L"      ╙─%ls───────────%ls─╜      ", border_left, border_right);

    free(border_left);
    free(border_right);

    runDialogBoxCallback(settings, dialog_box);
    con_flush();
}

void drawDialogBoxCentered(UserSettings * settings, DialogBox * dialog_box) {
    Rect centered = getScreenCenteredRect(dialog_box->width + 18, dialog_box->height + 6);
    drawDialogBox(settings, dialog_box, centered.x, centered.y);
}
