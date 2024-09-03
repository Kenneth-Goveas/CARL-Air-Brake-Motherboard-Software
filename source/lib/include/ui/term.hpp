#ifndef __UI_TERM_HPP__
#define __UI_TERM_HPP__

#include <string>

namespace term {

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
bool eof (void);

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

void newline (void);

template <typename ... types>
void print (types ... args);

template <typename type>
type scan (void);

template <>
bool scan<bool> (void);

template <>
std::string scan<std::string> (void);

template <>
int scan<int> (void);

template <>
double scan<double> (void);

}

#include <ui/term.tpp>

#endif
