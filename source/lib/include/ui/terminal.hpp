#ifndef __UI_TERMINAL_HPP__
#define __UI_TERMINAL_HPP__

namespace terminal {

typedef enum {
    black           =  0,
    darkred         =  1,
    darkgreen       =  2,
    darkyellow      =  3,
    darkblue        =  4,
    darkmagenta     =  5,
    darkcyan        =  6,
    lightgray       =  7,
    darkgray        =  8,
    lightred        =  9,
    lightgreen      = 10,
    lightyellow     = 11,
    lightblue       = 12,
    lightmagenta    = 13,
    lightcyan       = 14,
    white           = 15
} color;

bool fail (void);

void newline (void);

void erase_line (void);
void erase_screen (void);

void color_default (void);
void color_fore (color clr);
void color_back (color clr);

void cursor_hide (void);
void cursor_show (void);
void cursor_row (int row);
void cursor_col (int col);
void cursor_pos (int row, int col);

}

#endif
