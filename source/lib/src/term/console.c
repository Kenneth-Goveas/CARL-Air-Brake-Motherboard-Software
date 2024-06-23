#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>

#include <util/log.h>

#include <term/display.h>
#include <term/console.h>

LOG_DECLARE_MODULE("console");

static void _conv_color_to_str (char ** str, display_color_t color) {
    switch (color) {
        case DISPLAY_COLOR_BLACK:
            asprintf(str, "Black");
            break;
        case DISPLAY_COLOR_DARK_RED:
            asprintf(str, "Dark red");
            break;
        case DISPLAY_COLOR_DARK_GREEN:
            asprintf(str, "Dark green");
            break;
        case DISPLAY_COLOR_DARK_YELLOW:
            asprintf(str, "Dark yellow");
            break;
        case DISPLAY_COLOR_DARK_BLUE:
            asprintf(str, "Dark blue");
            break;
        case DISPLAY_COLOR_DARK_MAGENTA:
            asprintf(str, "Dark magenta");
            break;
        case DISPLAY_COLOR_DARK_CYAN:
            asprintf(str, "Dark cyan");
            break;
        case DISPLAY_COLOR_LIGHT_GRAY:
            asprintf(str, "Light gray");
            break;
        case DISPLAY_COLOR_DARK_GRAY:
            asprintf(str, "Dark gray");
            break;
        case DISPLAY_COLOR_LIGHT_RED:
            asprintf(str, "Light red");
            break;
        case DISPLAY_COLOR_LIGHT_GREEN:
            asprintf(str, "Light green");
            break;
        case DISPLAY_COLOR_LIGHT_YELLOW:
            asprintf(str, "Light yellow");
            break;
        case DISPLAY_COLOR_LIGHT_BLUE:
            asprintf(str, "Light blue");
            break;
        case DISPLAY_COLOR_LIGHT_MAGENTA:
            asprintf(str, "Light magenta");
            break;
        case DISPLAY_COLOR_LIGHT_CYAN:
            asprintf(str, "Light cyan");
            break;
        case DISPLAY_COLOR_WHITE:
            asprintf(str, "White");
            break;
    }
}

void console_init (void) {
    LOG_INF("Configuring terminal");

    display_unbuf();
    display_color_default();
    display_curs_hide();
    display_curs_col(1);
    display_erase_line();
}

void console_deinit (void) {
    LOG_INF("Reverting terminal");

    display_curs_col(1);
    display_curs_show();
    display_erase_line();
}

void console_new (void) {
    LOG_INF("Creating new line");
    display_write_newline();
}

void console_print (display_color_t color, uint8_t col, const char * fmt, ...) {
    va_list args;
    char * color_str;
    char * text_str;

    va_start(args, fmt);
    vasprintf(&text_str, fmt, args);
    va_end(args);

    _conv_color_to_str(&color_str, color);
    LOG_INF(
        "Printing: Color: %s, Col: %" PRIu8 ", Text: “%s”",
        color_str, col, text_str
    );
    free(color_str);

    display_curs_col(col);
    display_color_fore(color);
    display_write_text(text_str);
    display_color_default();

    free(text_str);
}
