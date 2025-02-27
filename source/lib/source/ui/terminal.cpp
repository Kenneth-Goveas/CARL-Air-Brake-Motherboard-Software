#include <iomanip>
#include <iostream>
#include <string>

#include <sys/logging.hpp>

#include <ui/terminal.hpp>

namespace terminal :: intern {

std::string mod = "terminal";

bool fail;

}

namespace terminal {

const color black(0, "Black");
const color white(15, "White");
const color darkgray(8, "Dark gray");
const color darkred(1, "Dark red");
const color darkgreen(2, "Dark green");
const color darkyellow(3, "Dark yellow");
const color darkblue(4, "Dark blue");
const color darkmagenta(5, "Dark magenta");
const color darkcyan(6, "Dark cyan");
const color lightgray(7, "Light gray");
const color lightred(9, "Light red");
const color lightgreen(10, "Light green");
const color lightyellow(11, "Light yellow");
const color lightblue(12, "Light blue");
const color lightmagenta(13, "Light magenta");
const color lightcyan(14, "Light cyan");

color :: color (int val, const char * text) {
    this->val = val;
    this->text = text;
}

color :: operator int (void) const {
    return this->val;
}

color :: operator logging::buffer (void) const {
    return logging::buffer(this->text);
}

bool fail (void) {
    return intern::fail;
}

void newline (void) {
    logging::inf(intern::mod,
        "Entering new line"
    );

    std::cout << std::endl;
}

void erase_line (void) {
    logging::inf(intern::mod,
        "Erasing line"
    );

    std::cout << "\e[2K";
    std::cout.flush();
}

void erase_screen (void) {
    logging::inf(intern::mod,
        "Erasing screen"
    );

    std::cout << "\e[2J";
    std::cout.flush();
}

void color_default (void) {
    logging::inf(intern::mod,
        "Setting default colors"
    );

    std::cout << "\e[m";
    std::cout.flush();
}

void color_fore (color color) {
    logging::inf(intern::mod,
        "Setting foreground color: ", color
    );

    std::cout << "\e[38;5;" << int(color) << "m";
    std::cout.flush();
}

void color_back (color color) {
    logging::inf(intern::mod,
        "Setting background color: ", color
    );

    std::cout << "\e[48;5;" << int(color) << "m";
    std::cout.flush();
}

void cursor_hide (void) {
    logging::inf(intern::mod,
        "Hiding cursor"
    );

    std::cout << "\e[?25l";
    std::cout.flush();
}

void cursor_show (void) {
    logging::inf(intern::mod,
        "Revealing cursor"
    );

    std::cout << "\e[?25h";
    std::cout.flush();
}

void cursor_row (int row) {
    logging::inf(intern::mod,
        "Moving cursor: Row: ", row
    );

    if (row <= 0) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << row << "d";
    std::cout.flush();

    intern::fail = false;
}

void cursor_col (int col) {
    logging::inf(intern::mod,
        "Moving cursor: Col: ", col
    );

    if (col <= 0) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << col << "G";
    std::cout.flush();

    intern::fail = false;
}

void cursor_pos (int row, int col) {
    logging::inf(intern::mod,
        "Moving cursor: Row: ", row, ", Col: ", col
    );

    if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to move cursor (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    std::cout << "\e[" << row << ";" << col << "H";
    std::cout.flush();

    intern::fail = false;
}

}
