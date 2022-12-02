#ifndef CON_LIB_H
#define CON_LIB_H

// Spalvos
#define COLOR_BLACK     0
#define COLOR_RED       1
#define COLOR_GREEN     2
#define COLOR_ORANGE    3
#define COLOR_BLUE      4
#define COLOR_MAGENTA   5
#define COLOR_CYAN      6
#define COLOR_GRAY      7
#define COLOR_RESET     9

// Ekrano dydis
struct ConSize{
    int height, width;
};

// Išvalo ekraną
void con_clear();

// Nuskaito vieną klavišą. 
// Gražina 0, jei nėra ko daugiau skaityti
int con_read_key();

// Nustato fono ir teksto spalvą
// * bg - fono spalva (COLOR_*)
// * fg - teksto spalva (COLOR_*)
void con_set_color(int bg, int fg);

// Nustato dabartinę išvedimo poziciją. x, y - koordinatės:
// * viršutinis kairys terminalo kampas yra (0, 0)
// * x-ai didėja iš kairės į dešinę
// * y-ai didėja iš viršaus į apačią
void con_set_pos(int x, int y);

// Nustato cursoriaus rodymo rėžimą.
// * Jei show == 0, cursorių paslepia
// * Jei show == 1, cursorių rodo
void con_show_cursor(int show);

// Padaro, kad nesimatytų įvedamų simbolių (tik LINUX os)
// * Jei show == 0, tai įvedamų simbolių neatkartoja į ekraną
// * Jei show == 1, tai įvedamus simbolius rodo 
// * Ant Windows nieko nedaro 
void con_show_echo(int show);

// Miega nurodytą sekundžių skaičių.
// * seconds turi būti intervale [0.01; 100.0]
void con_sleep(float seconds);

// Grąžina ekrano dydį
struct ConSize con_get_size();

#endif // CON_LIB_H
