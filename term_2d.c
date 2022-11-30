#include "ui/con_lib.h"

// Piešia staciakampį
// x, y - viršutinio kairio kampo koordinatės
// w, h - stačiakampio plotis (width) ir aukštis (height)
void draw_box(int x, int y, int w, int h) {
  for (int i = 0; i < w; i++) {
    con_set_pos(x + i, y);
    printf("#");

    con_set_pos(x + i, y + h - 1);
    printf("#");
  }

  for (int i = 0; i < h; i++) {
    con_set_pos(x, y + i);
    printf("#");

    con_set_pos(x + w - 1, y + i);
    printf("#");
  }
}

int main(int argc, char** argv) {
  // Paslepia įvedamus simbolius
  con_show_echo(0);

  // Paslepia cursorių
  con_show_cursor(0);

  // Sena pozicija
  int old_x = 10, old_y = 10;

  // Judėjimo kryptis
  int dir_x = 1, dir_y = 0;

  // Dabartinė pozicija
  int pos_x = 10, pos_y = 10;

  // Piešiam rėmelį
  con_clear();
  draw_box(0, 0, 40, 20);

  while (1) {
    int key = 0;

    // Nuskaitom visus paspaustus klavišus iš klavietūros
    while (key = con_read_key()) {
      if (key == 'w') {
        dir_x = 0;
        dir_y = -1;
      } else if (key == 's') {
        dir_x = 0;
        dir_y = 1;
      } else if (key == 'a') {
        dir_x = -1;
        dir_y = 0;
      } else if (key == 'd') {
        dir_x = 1;
        dir_y = 0;
      } else if (key == 'q') {
        // Jei paspausta q, tai išeinam iš programos
        return 0;
      }
    }

    // Atnaujinam X poziciją: pasiekus ekrano galą, peršokam į priešingą pusę
    pos_x += dir_x;
    if (pos_x < 1) 
      pos_x = 38;
    else if (pos_x > 38)
      pos_x = 1;

    // Atnaujinam Y poziciją: pasiekus ekrano galą, peršokam į priešingą pusę
    pos_y += dir_y;
    if (pos_y < 1) 
      pos_y = 18;
    else if (pos_y > 18)
      pos_y = 1;

    // Ištriniam seną simbolį
    con_set_color(COLOR_BLACK, COLOR_GRAY);
    con_set_pos(old_x, old_y);
    printf(" ");

    // Nupiešiam naują
    con_set_color(COLOR_RED, COLOR_RED);
    con_set_pos(pos_x, pos_y); 
    printf("#");
    fflush(stdout);

    // Išsaugom senas koordinates
    old_x = pos_x;
    old_y = pos_y;

    // Pamiegam 0.08 sekundės
    con_sleep(0.08);
  }
}
