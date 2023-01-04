#ifndef CHESS_RECT_H
#define CHESS_RECT_H

typedef struct Rect {
    int x, y, width, height;
} Rect;

Rect createRect(int x, int y, int width, int height);

Rect centerRectInRect(int inner_width, int inner_height, Rect outer);
Rect getCenteredRect(int width, int height);

#endif //CHESS_RECT_H
