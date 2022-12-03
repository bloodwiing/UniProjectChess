#include "con_lib.h"
#include <wchar.h>

// Bibliotekos realizacija. TOP SECRET
#ifdef __linux__

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

void con_setup_reset();

void con_clear() {
  wprintf(L"\x1B[0m"); // reset colour
  wprintf(L"\x1B[1;1H"); // reset position
  wprintf(L"\x1B[2J"); // clear screen
}

void con_handle_abort() {
  con_setup_reset();
  exit(-1);
}

uint32_t con_read_key() {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  struct timeval tv = { 0L, 0L };
  if (select(1, &fds, NULL, NULL, &tv)) {
    uint32_t res = 0;
    unsigned char c[4];
    size_t size;
    if ((size = read(0, &c, sizeof(c))) > 0) {
      for (int i = 0; i < size; i++) {
        unsigned char single = c[i];
        if (single >= 65 && single <= 90)  // turn uppercase to lowercase
          single += 32;
        res <<= 8;
        res |= single;
      }
      return res;
    }
  }

  return 0;
}

void con_set_color(int bg, int fg) {
  if (bg & COLOR_INTENSE) {
    switch (bg & ~COLOR_INTENSE & 0b10111) {
      case COLOR_BLACK:   wprintf(L"\x1B[100m"); break;
      case COLOR_RED:     wprintf(L"\x1B[101m"); break;
      case COLOR_GREEN:   wprintf(L"\x1B[102m"); break;
      case COLOR_ORANGE:  wprintf(L"\x1B[103m"); break;
      case COLOR_BLUE:    wprintf(L"\x1B[104m"); break;
      case COLOR_MAGENTA: wprintf(L"\x1B[105m"); break;
      case COLOR_CYAN:    wprintf(L"\x1B[106m"); break;
      case COLOR_GRAY:    wprintf(L"\x1B[107m"); break;
      case COLOR_RESET:   wprintf(L"\x1B[49m"); break;
    }
  } else {
    switch (bg) {
      case COLOR_BLACK:   wprintf(L"\x1B[40m"); break;
      case COLOR_RED:     wprintf(L"\x1B[41m"); break;
      case COLOR_GREEN:   wprintf(L"\x1B[42m"); break;
      case COLOR_ORANGE:  wprintf(L"\x1B[43m"); break;
      case COLOR_BLUE:    wprintf(L"\x1B[44m"); break;
      case COLOR_MAGENTA: wprintf(L"\x1B[45m"); break;
      case COLOR_CYAN:    wprintf(L"\x1B[46m"); break;
      case COLOR_GRAY:    wprintf(L"\x1B[47m"); break;
      case COLOR_RESET:   wprintf(L"\x1B[49m"); break;
    }
  }

  if (fg & COLOR_INTENSE) {
    switch (fg & ~COLOR_INTENSE & 0b10111) {
      case COLOR_BLACK:   wprintf(L"\x1B[90m"); break;
      case COLOR_RED:     wprintf(L"\x1B[91m"); break;
      case COLOR_GREEN:   wprintf(L"\x1B[92m"); break;
      case COLOR_ORANGE:  wprintf(L"\x1B[93m"); break;
      case COLOR_BLUE:    wprintf(L"\x1B[94m"); break;
      case COLOR_MAGENTA: wprintf(L"\x1B[95m"); break;
      case COLOR_CYAN:    wprintf(L"\x1B[96m"); break;
      case COLOR_GRAY:    wprintf(L"\x1B[97m"); break;
      case COLOR_RESET:   wprintf(L"\x1B[39m"); break;
    }
  } else {
    switch (fg) {
      case COLOR_BLACK:   wprintf(L"\x1B[30m"); break;
      case COLOR_RED:     wprintf(L"\x1B[31m"); break;
      case COLOR_GREEN:   wprintf(L"\x1B[32m"); break;
      case COLOR_ORANGE:  wprintf(L"\x1B[33m"); break;
      case COLOR_BLUE:    wprintf(L"\x1B[34m"); break;
      case COLOR_MAGENTA: wprintf(L"\x1B[35m"); break;
      case COLOR_CYAN:    wprintf(L"\x1B[36m"); break;
      case COLOR_GRAY:    wprintf(L"\x1B[37m"); break;
      case COLOR_RESET:   wprintf(L"\x1B[39m"); break;
    }
  }
}

void con_set_pos(int x, int y) {
  if (x >= 0 && y >= 0 && x < 400 && y < 200) {
    wprintf(L"\x1B[%i;%iH", y + 1, x + 1);
  }
}

void con_setup_reset() {
  con_show_echo(1);
  con_show_cursor(1);
  con_clear();
}

void con_show_cursor(int show) {
  if (show) {
    wprintf(L"\x1B[?25h");
  } else {
    wprintf(L"\x1B[?25l");
  }
}

void con_show_echo(int show) {
  struct termios termios;

  tcgetattr(STDIN_FILENO, &termios);
  if (show)
    termios.c_lflag |= ICANON | ECHO;
  else
    termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &termios);

  static int initialized = 0;
  if (!initialized) {
    atexit(con_setup_reset);
    signal(SIGINT, con_handle_abort);
    signal(SIGTERM, con_handle_abort);
    initialized = 1;
  }
}

void con_sleep(float seconds) {
  if (seconds >= 0.01f && seconds <= 100.0f) {
    usleep((useconds_t) (seconds * 1e6f));
  }
}

struct ConSize con_get_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);

    struct ConSize result;
    result.width = w.ws_col;
    result.height = w.ws_row;
    return result;
}

#elif _WIN32

#include <stdio.h>
#include <windows.h>

HANDLE con_get_stdin();
HANDLE con_get_stdout();

void con_clear() {
  HANDLE handle = con_get_stdout();

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(handle, &csbi)) {
    return;
  }

  COORD coordScreen = { 0, 0 };
  SetConsoleCursorPosition(handle, coordScreen);
  SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
  
  DWORD cCharsWritten;
  DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(handle, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
  FillConsoleOutputAttribute(handle, 0, dwConSize, coordScreen, &cCharsWritten);
}

uint32_t con_read_key() {
  HANDLE handle = con_get_stdin();
  INPUT_RECORD input_record;
  DWORD num_events = 0;
  DWORD num_events_read;

  GetNumberOfConsoleInputEvents(handle, &num_events);

  for (; num_events > 0; num_events--) {
    if (!ReadConsoleInput(handle, &input_record, 1, &num_events_read)) {
      break;
    }

    if (input_record.EventType == KEY_EVENT && input_record.Event.KeyEvent.bKeyDown == TRUE) {
      return input_record.Event.KeyEvent.wVirtualKeyCode;
    }
  }

  return 0;
}

// Used for resetting to original colours
WORD * defaultAttr = NULL;

#define FOREGROUND_BYTES (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define BACKGROUND_BYTES (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)

void con_set_color(int bg, int fg) {
  DWORD attr = 0;

  if (defaultAttr == NULL) {
    defaultAttr = malloc(sizeof(WORD));
    // https://stackoverflow.com/questions/25559077/how-to-get-background-color-back-to-previous-color-after-use-of-std-handle/25560218#25560218
    CONSOLE_SCREEN_BUFFER_INFO Info;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &Info);
    *defaultAttr = Info.wAttributes;
  }

  if (bg & COLOR_INTENSE) attr |= BACKGROUND_INTENSITY;
  switch (bg & ~COLOR_INTENSE & 0b10111) {
    case COLOR_RED:
      attr |= BACKGROUND_RED;
      break;

    case COLOR_GREEN:
      attr |= BACKGROUND_GREEN;
      break;

    case COLOR_ORANGE:  
      attr |= BACKGROUND_RED | BACKGROUND_GREEN;
      break;

    case COLOR_BLUE:
      attr |= BACKGROUND_BLUE;
      break;

    case COLOR_MAGENTA: 
      attr |= BACKGROUND_RED | BACKGROUND_BLUE; 
      break;

    case COLOR_CYAN:
      attr |= BACKGROUND_GREEN | BACKGROUND_BLUE;
      break;

    case COLOR_LIGHT_GRAY:
      attr |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
      break;

    case COLOR_RESET:
      attr = *defaultAttr & BACKGROUND_BYTES;
      break;
  }

  if (fg & COLOR_INTENSE) attr |= FOREGROUND_INTENSITY;
  long a = fg & ~COLOR_INTENSE & 0b10111;
  switch (fg & ~COLOR_INTENSE & 0b10111) {
    case COLOR_RED:
      attr |= FOREGROUND_RED;
      break;

    case COLOR_GREEN:
      attr |= FOREGROUND_GREEN;
      break;

    case COLOR_ORANGE:  
      attr |= FOREGROUND_RED | FOREGROUND_GREEN;
      break;

    case COLOR_BLUE:
      attr |= FOREGROUND_BLUE;
      break;

    case COLOR_MAGENTA: 
      attr |= FOREGROUND_RED | FOREGROUND_BLUE; 
      break;

    case COLOR_CYAN:
      attr |= FOREGROUND_GREEN | FOREGROUND_BLUE;
      break;

    case COLOR_LIGHT_GRAY:
      attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
      break;

    case COLOR_RESET:
      attr = (*defaultAttr & FOREGROUND_BYTES) | (attr & BACKGROUND_BYTES);
      break;
  }

  SetConsoleTextAttribute(con_get_stdout(), attr);
}

void con_set_pos(int x, int y) {
  if (x >= 0 && y >= 0 && x < 400 && y < 200) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(con_get_stdout(), pos);
  }
}

void con_show_cursor(int show) {
  CONSOLE_CURSOR_INFO cursor_info;

  cursor_info.dwSize = 1;
  cursor_info.bVisible = show ? 1 : 0;
  SetConsoleCursorInfo(con_get_stdout(), &cursor_info);
}

HANDLE con_get_stdin() {
  static HANDLE handle = 0;
  if (!handle) {
    handle = GetStdHandle(STD_INPUT_HANDLE);
  }
  return handle;
}

HANDLE con_get_stdout() {
  static HANDLE handle = 0;
  if (!handle) {
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
  }
  return handle;
}

void con_show_echo(int show) {
  /*HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 

  DWORD fdwSaveOldMode;
  GetConsoleMode(hStdin, &fdwSaveOldMode);
 
  DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT; 
  SetConsoleMode(hStdin, fdwMode);*/
}

void con_sleep(float seconds) {
  if (seconds >= 0.01f && seconds <= 100.0f) {
    Sleep((DWORD) (seconds * 1e3f));
  }
}

// Modified version of https://stackoverflow.com/questions/6812224/getting-terminal-size-in-c-for-windows/12642749#12642749
struct ConSize con_get_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    struct ConSize size;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return size;
}

#endif // _WIN32
