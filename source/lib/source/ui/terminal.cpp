#include <iomanip>
#include <iostream>
#include <string>

#include <sys/logging.hpp>

#include <ui/terminal.hpp>

namespace terminal :: intern {

std::string mod = "terminal";

bool fail;

std::string conv_color_to_oupt (color clr);

}

namespace terminal {

bool fail (void) {
    return intern::fail;
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

void color_fore (color clr) {
    std::string oupt;

    oupt = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Setting foreground color: ", oupt
    );

    std::cout << "\e[38;5;" << clr << "m";
    std::cout.flush();
}

void color_back (color clr) {
    std::string oupt;

    oupt = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Setting background color: ", oupt
    );

    std::cout << "\e[48;5;" << clr << "m";
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

    if (!(row > 0)) {
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

    if (!(col > 0)) {
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

    if (!(row > 0 && col > 0)) {
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

namespace terminal :: intern {

std::string conv_color_to_oupt (color clr) {
    switch (clr) {
        case black:
            return std::string("Black");
        case darkred:
            return std::string("Dark red");
        case darkgreen:
            return std::string("Dark green");
        case darkyellow:
            return std::string("Dark yellow");
        case darkblue:
            return std::string("Dark blue");
        case darkmagenta:
            return std::string("Dark magenta");
        case darkcyan:
            return std::string("Dark cyan");
        case lightgray:
            return std::string("Light gray");
        case darkgray:
            return std::string("Dark gray");
        case lightred:
            return std::string("Light red");
        case lightgreen:
            return std::string("Light green");
        case lightyellow:
            return std::string("Light yellow");
        case lightblue:
            return std::string("Light blue");
        case lightmagenta:
            return std::string("Light magenta");
        case lightcyan:
            return std::string("Light cyan");
        case white:
            return std::string("White");
        default:
            return std::string("");
    }
}

}
