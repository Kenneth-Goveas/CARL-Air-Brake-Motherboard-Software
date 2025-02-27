#ifndef __UI_TERMINAL_HPP__
#define __UI_TERMINAL_HPP__

#include <sys/logging.hpp>

namespace terminal {

class color {
    public:
        color (int val, const char * text);

        operator int (void) const;
        operator logging::buffer (void) const;

    private:
        int val;
        const char * text;
};

extern const color black;
extern const color white;
extern const color darkgray;
extern const color darkred;
extern const color darkgreen;
extern const color darkyellow;
extern const color darkblue;
extern const color darkmagenta;
extern const color darkcyan;
extern const color lightgray;
extern const color lightred;
extern const color lightgreen;
extern const color lightyellow;
extern const color lightblue;
extern const color lightmagenta;
extern const color lightcyan;

bool fail (void);

void newline (void);

void erase_line (void);
void erase_screen (void);

void color_default (void);
void color_fore (color color);
void color_back (color color);

void cursor_hide (void);
void cursor_show (void);
void cursor_row (int row);
void cursor_col (int col);
void cursor_pos (int row, int col);

}

#endif
