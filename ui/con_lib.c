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

int con_read_key() {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  struct timeval tv = { 0L, 0L };
  if (select(1, &fds, NULL, NULL, &tv)) {
    unsigned char c;
    size_t size;
    if ((size = read(0, &c, sizeof(c))) > 0) {
      return c;
    }
  }

  return 0;
}

void con_set_color(int bg, int fg) {
  switch (bg) {
    case COLOR_BLACK:   wprintf(L"\x1B[40m"); break;
    case COLOR_RED:     wprintf(L"\x1B[41m"); break;
    case COLOR_GREEN:   wprintf(L"\x1B[42m"); break;
    case COLOR_ORANGE:  wprintf(L"\x1B[43m"); break;
    case COLOR_BLUE:    wprintf(L"\x1B[44m"); break;
    case COLOR_MAGENTA: wprintf(L"\x1B[45m"); break;
    case COLOR_CYAN:    wprintf(L"\x1B[46m"); break;
    case COLOR_GRAY:    wprintf(L"\x1B[47m"); break;
  }

  switch (fg) {
    case COLOR_BLACK:   wprintf(L"\x1B[30m"); break;
    case COLOR_RED:     wprintf(L"\x1B[31m"); break;
    case COLOR_GREEN:   wprintf(L"\x1B[32m"); break;
    case COLOR_ORANGE:  wprintf(L"\x1B[33m"); break;
    case COLOR_BLUE:    wprintf(L"\x1B[34m"); break;
    case COLOR_MAGENTA: wprintf(L"\x1B[35m"); break;
    case COLOR_CYAN:    wprintf(L"\x1B[36m"); break;
    case COLOR_GRAY:    wprintf(L"\x1B[37m"); break;
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

int con_read_key() {
  HANDLE handle = con_get_stdin();
  INPUT_RECORD input_record;
  DWORD num_events = 0;
  DWORD num_events_read;

  GetNumberOfConsoleInputEvents(handle, &num_events);

  for (; num_events > 0; num_events--) {
    if (!ReadConsoleInput(handle, &input_record, 1, &num_events_read)) {
      break;
    }

    if (input_record.EventType == KEY_EVENT) {
      return input_record.Event.KeyEvent.uChar.AsciiChar;
    }
  }

  return 0;
}

void con_set_color(int bg, int fg) {
  DWORD attr = 0;

  switch (bg) {
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

    case COLOR_GRAY:
      attr |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
      break;
  }

  switch (fg) {
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

    case COLOR_GRAY:
      attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
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

#endif // _WIN32
