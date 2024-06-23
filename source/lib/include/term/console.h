#ifndef __TERM_CONSOLE_H__
#define __TERM_CONSOLE_H__

#include <stdint.h>

#include <term/display.h>

void console_init (void);
void console_deinit (void);
void console_new (void);
void console_print (display_color_t color, uint8_t col, const char * fmt, ...);

#endif
