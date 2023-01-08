#ifndef CHESS_DIALOGBOX_H
#define CHESS_DIALOGBOX_H

#include <stdint.h>
#include <wchar.h>

#include "settings/settings.h"

#define DIALOGBOX_MAX_TITLE_LEN 100
#define DIALOGBOX_CONTENT_CALLBACK(name) void (name)(UserSettings * settings, int x, int y, int width, int height, void * data_ptr)

typedef struct DialogBox {
    wchar_t title[DIALOGBOX_MAX_TITLE_LEN];
    uint16_t height, width;

    uint16_t x, y;

    DIALOGBOX_CONTENT_CALLBACK(*content_callback);

    void * data_ptr;
} DialogBox;

/// Creates a new dialog box
DialogBox createDialogBox(wchar_t * title, uint16_t height, uint16_t width, DIALOGBOX_CONTENT_CALLBACK(*content_callback));


/// Runs the dialog box's callback
void runDialogBoxCallback(UserSettings * settings, DialogBox * dialog_box);


/// Draws the dialog box to the screen at the given position
void drawDialogBox(UserSettings * settings, DialogBox * dialog_box, int x, int y);

/// Draws the dialog box centered to the console's screen
void drawDialogBoxCentered(UserSettings * settings, DialogBox * dialog_box);

#endif //CHESS_DIALOGBOX_H
