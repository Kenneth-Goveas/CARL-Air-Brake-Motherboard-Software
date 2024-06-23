#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

#include <util/log.h>

#include <term/display.h>
#include <term/panel.h>

LOG_DECLARE_MODULE("panel");

static uint8_t * _row = NULL;
static uint8_t * _col = NULL;
static uint8_t * _hght = NULL;
static uint8_t * _wdth = NULL;
static display_color_t * _color = NULL;
static int _count = 0;

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

void panel_init (void) {
    LOG_INF("Configuring terminal");

    display_unbuf();
    display_color_default();
    display_curs_hide();
    display_curs_pos(1, 1);
    display_erase_screen();
}

void panel_deinit (void) {
    LOG_INF("Reverting terminal");

    display_curs_pos(1, 1);
    display_curs_show();
    display_erase_screen();

    free(_row);
    free(_col);
    free(_hght);
    free(_wdth);
    free(_color);

    _row = NULL;
    _col = NULL;
    _hght = NULL;
    _wdth = NULL;
    _color = NULL;
    _count = 0;
}

int panel_new (
    display_color_t color, uint8_t row, uint8_t col, uint8_t hght, uint8_t wdth
) {
    char * color_str;
    char * blank_str;

    _conv_color_to_str(&color_str, color);
    LOG_INF(
        "Creating new panel #%d: "
        "Color: %s, "
        "Row: %" PRIu8 ", Col: %" PRIu8 ", "
        "Hght: %" PRIu8 ", Wdth: %" PRIu8,
        _count + 1, color_str, row, col, hght, wdth
    );
    free(color_str);

    _count++;

    _row = (uint8_t *)realloc(_row, _count);
    _col = (uint8_t *)realloc(_col, _count);
    _hght = (uint8_t *)realloc(_hght, _count);
    _wdth = (uint8_t *)realloc(_wdth, _count);
    _color = (display_color_t *)realloc(
        _color, _count * sizeof(display_color_t)
    );

    _row[_count - 1] = row;
    _col[_count - 1] = col;
    _hght[_count - 1] = hght;
    _wdth[_count - 1] = wdth;
    _color[_count - 1] = color;

    blank_str = (char *)malloc((wdth + 1));

    memset(blank_str, ' ', wdth);
    blank_str[wdth] = '\0';

    display_color_back(color);
    for (uint8_t i = 0; i < hght; i++) {
        display_curs_pos(row + i, col);
        display_write_text(blank_str);
    }
    display_color_default();

    free(blank_str);

    return (_count - 1);
}

void panel_print (
    int id, display_color_t color, uint8_t row, uint8_t col,
    const char * fmt, ...
) {
    va_list args;
    char * color_str;
    char * text_str;

    va_start(args, fmt);
    vasprintf(&text_str, fmt, args);
    va_end(args);

    _conv_color_to_str(&color_str, color);
    LOG_INF(
        "Printing on panel #%d: "
        "Color: %s, Row: %" PRIu8 ", Col: %" PRIu8 ", Text: “%s”",
        id + 1, color_str, row, col, text_str
    );
    free(color_str);

    display_color_fore(color);
    display_color_back(_color[id]);
    display_curs_pos(row + _row[id] - 1, col + _col[id] - 1);
    display_write_text(text_str);
    display_color_default();

    free(text_str);
}
