#ifndef __TERM_PANEL_H__
#define __TERM_PANEL_H__

#include <stdint.h>

#include <term/display.h>

void panel_init (void);
void panel_deinit (void);
int panel_new (
    display_color_t color, uint8_t row, uint8_t col, uint8_t hght, uint8_t wdth
);
void panel_print (
    int id, display_color_t color, uint8_t row, uint8_t col,
    const char * fmt, ...
);

#endif
