#ifndef __TERM_DISPLAY_H__
#define __TERM_DISPLAY_H__

#include <stdint.h>

typedef enum {
    DISPLAY_COLOR_BLACK         = 0,
    DISPLAY_COLOR_DARK_RED      = 1,
    DISPLAY_COLOR_DARK_GREEN    = 2,
    DISPLAY_COLOR_DARK_YELLOW   = 3,
    DISPLAY_COLOR_DARK_BLUE     = 4,
    DISPLAY_COLOR_DARK_MAGENTA  = 5,
    DISPLAY_COLOR_DARK_CYAN     = 6,
    DISPLAY_COLOR_LIGHT_GRAY    = 7,
    DISPLAY_COLOR_DARK_GRAY     = 8,
    DISPLAY_COLOR_LIGHT_RED     = 9,
    DISPLAY_COLOR_LIGHT_GREEN   = 10,
    DISPLAY_COLOR_LIGHT_YELLOW  = 11,
    DISPLAY_COLOR_LIGHT_BLUE    = 12,
    DISPLAY_COLOR_LIGHT_MAGENTA = 13,
    DISPLAY_COLOR_LIGHT_CYAN    = 14,
    DISPLAY_COLOR_WHITE         = 15
} display_color_t;

void display_unbuf (void);
void display_erase_line (void);
void display_erase_screen (void);
void display_color_default (void);
void display_color_fore (display_color_t color);
void display_color_back (display_color_t color);
void display_curs_hide (void);
void display_curs_show (void);
void display_curs_row (uint8_t row);
void display_curs_col (uint8_t col);
void display_curs_pos (uint8_t row, uint8_t col);
void display_write_text (const char * str);
void display_write_newline (void);

#endif
