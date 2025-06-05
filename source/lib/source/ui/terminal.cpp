#include <iomanip>
#include <iostream>

#include <sys/logging.hpp>
#include <ui/terminal.hpp>

namespace terminal {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (void) : logging("terminal"), init(false) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->init = true;
}

void instance::erase (void) {
    this->logging.inf("Erasing screen");

    if (!this->init) {
        this->logging.err("Failed to erase screen (Instance not initialized)");
        throw except_erase::fail;
    }

    std::cout << "\x1B[2J";
    std::cout.flush();
}

void instance::color_default (void) {
    this->logging.inf("Setting default colors");

    if (!this->init) {
        this->logging.err("Failed to set default colors (Instance not initialized)");
        throw except_color_default::fail;
    }

    std::cout << "\x1B[m";
    std::cout.flush();
}

void instance::color_fore (color color) {
    this->logging.inf("Setting foreground color: ", color);

    if (!this->init) {
        this->logging.err("Failed to set foreground color (Instance not initialized)");
        throw except_color_fore::fail;
    }

    std::cout << "\x1B[38;5;" << int(color) << "m";
    std::cout.flush();
}

void instance::color_back (color color) {
    this->logging.inf("Setting background color: ", color);

    if (!this->init) {
        this->logging.err("Failed to set background color (Instance not initialized)");
        throw except_color_back::fail;
    }

    std::cout << "\x1B[48;5;" << int(color) << "m";
    std::cout.flush();
}

void instance::cursor_hide (void) {
    this->logging.inf("Hiding cursor");

    if (!this->init) {
        this->logging.err("Failed to hide cursor (Instance not initialized)");
        throw except_cursor_hide::fail;
    }

    std::cout << "\x1B[?25l";
    std::cout.flush();
}

void instance::cursor_show (void) {
    this->logging.inf("Revealing cursor");

    if (!this->init) {
        this->logging.err("Failed to reveal cursor (Instance not initialized)");
        throw except_cursor_show::fail;
    }

    std::cout << "\x1B[?25h";
    std::cout.flush();
}

void instance::cursor_move (int row, int col) {
    this->logging.inf("Moving cursor: Row: ", row, ", Col: ", col);

    if (!this->init) {
        this->logging.err("Failed to move cursor (Instance not initialized)");
        throw except_cursor_move::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to move cursor (Invalid coordinates)");
        throw except_cursor_move::fail;
    }

    std::cout << "\x1B[" << row << ";" << col << "H";
    std::cout.flush();
}

logging::stream & operator << (logging::stream & strm, color color) {
    switch (color) {
        case color::black:
            strm << "Black";
            return strm;
        case color::white:
            strm << "White";
            return strm;
        case color::darkgray:
            strm << "Dark gray";
            return strm;
        case color::darkred:
            strm << "Dark red";
            return strm;
        case color::darkgreen:
            strm << "Dark green";
            return strm;
        case color::darkyellow:
            strm << "Dark yellow";
            return strm;
        case color::darkblue:
            strm << "Dark blue";
            return strm;
        case color::darkmagenta:
            strm << "Dark magenta";
            return strm;
        case color::darkcyan:
            strm << "Dark cyan";
            return strm;
        case color::lightgray:
            strm << "Light gray";
            return strm;
        case color::lightred:
            strm << "Light red";
            return strm;
        case color::lightgreen:
            strm << "Light green";
            return strm;
        case color::lightyellow:
            strm << "Light yellow";
            return strm;
        case color::lightblue:
            strm << "Light blue";
            return strm;
        case color::lightmagenta:
            strm << "Light magenta";
            return strm;
        case color::lightcyan:
            strm << "Light cyan";
            return strm;
        default:
            return strm;
    }
}

}
