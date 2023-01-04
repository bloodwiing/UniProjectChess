#ifndef CHESS_KEY_H
#define CHESS_KEY_H

#ifdef _WIN32

#define KEY_ENTER 13
#define KEY_SPACE 0x20

#define KEY_ARROW_UP 38
#define KEY_ARROW_DOWN 40
#define KEY_ARROW_LEFT 37
#define KEY_ARROW_RIGHT 39

#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68

#define KEY_Q 81

#define KEY_ESCAPE 0x1B

#elif __linux__

#define KEY_ENTER 10
#define KEY_SPACE need_value

#define KEY_ARROW_UP 1792865
#define KEY_ARROW_DOWN 1792866
#define KEY_ARROW_LEFT 1792868
#define KEY_ARROW_RIGHT 1792867

#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#define KEY_Q 113

#define KEY_ESCAPE need_value

#endif

#define CASE_KEY_CONFIRM case KEY_ENTER: \
case KEY_SPACE

#define CASE_KEY_UP case KEY_ARROW_UP: \
case KEY_W

#define CASE_KEY_DOWN case KEY_ARROW_DOWN: \
case KEY_S

#define CASE_KEY_LEFT case KEY_ARROW_LEFT: \
case KEY_A

#define CASE_KEY_RIGHT case KEY_ARROW_RIGHT: \
case KEY_D

#define CASE_KEY_CANCEL case KEY_Q: \
case KEY_ESCAPE

#endif //CHESS_KEY_H
