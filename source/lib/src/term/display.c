#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#include <util/log.h>

#include <term/display.h>

LOG_DECLARE_MODULE("display");

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

void display_unbuf (void) {
    LOG_INF("Disabling buffering");
    setbuf(stdout, NULL);
}

void display_erase_line (void) {
    LOG_INF("Erasing line");
    printf("\e[2K");
}

void display_erase_screen (void) {
    LOG_INF("Erasing screen");
    printf("\e[2J");
}

void display_color_default (void) {
    LOG_INF("Setting default colors");
    printf("\e[m");
}

void display_color_fore (display_color_t color) {
    char * color_str;

    _conv_color_to_str(&color_str, color);
    LOG_INF("Setting foreground color: %s", color_str);
    free(color_str);

    printf("\e[38;5;%dm", (int)color);
}

void display_color_back (display_color_t color) {
    char * color_str;

    _conv_color_to_str(&color_str, color);
    LOG_INF("Setting background color: %s", color_str);
    free(color_str);

    printf("\e[48;5;%dm", (int)color);
}

void display_curs_hide (void) {
    LOG_INF("Hiding cursor");
    printf("\e[?25l");
}

void display_curs_show (void) {
    LOG_INF("Revealing cursor");
    printf("\e[?25h");
}

void display_curs_row (uint8_t row) {
    LOG_INF("Moving cursor: Row: %" PRIu8, row);
    printf("\e[%" PRIu8 "d", row);
}

void display_curs_col (uint8_t col) {
    LOG_INF("Moving cursor: Col: %" PRIu8, col);
    printf("\e[%" PRIu8 "G", col);
}

void display_curs_pos (uint8_t row, uint8_t col) {
    LOG_INF("Moving cursor: Row: %" PRIu8 ", Col: %" PRIu8, row, col);
    printf("\e[%" PRIu8 ";%" PRIu8 "H", row, col);
}

void display_write_text (const char * str) {
    LOG_INF("Writing text: “%s”", str);
    printf("%s", str);
}

void display_write_newline (void) {
    LOG_INF("Writing line feed");
    printf("\n");
}
