#include <sys/logging.hpp>
#include <ui/terminal.hpp>
#include <ui/screen.hpp>

namespace screen {

bool tracker::exist = false;

bool tracker::instantiate (void) {
    if (tracker::exist) {
        return false;
    } else {
        tracker::exist = true;
        return true;
    }
}

instance::instance (terminal::instance & terminal) : logging("screen"), init(false), terminal(&terminal) {
    this->logging.inf("Initializing instance");

    if (!instance::instantiate()) {
        this->logging.err("Failed to initialize instance (Instance already exists)");
        throw except_ctor::fail;
    }

    this->logging.inf("Configuring screen");
    try {
        this->terminal->cursor_move(1, 1);
        this->terminal->cursor_hide();
        this->terminal->color_default();
        this->terminal->erase();
    } catch (...) {
        this->logging.err("Failed to configure screen");
        this->logging.err("Failed to initialize instance");
        throw except_ctor::fail;
    }

    this->init = true;
}

instance::~instance (void) {
    if (this->init) {
        this->logging.inf("Reverting screen");
        try {
            this->terminal->cursor_move(1, 1);
            this->terminal->cursor_show();
            this->terminal->color_default();
            this->terminal->erase();
        } catch (...) {
            this->logging.err("Failed to revert screen");
        }
    }
}

void instance::erase (void) {
    this->logging.inf("Erasing screen");

    if (!this->init) {
        this->logging.err("Failed to erase screen (Instance not initialized)");
        throw except_erase::fail;
    }

    try {
        this->terminal->cursor_move(1, 1);
        this->terminal->cursor_hide();
        this->terminal->color_default();
        this->terminal->erase();
    } catch (...) {
        this->logging.err("Failed to erase screen");
        throw except_erase::fail;
    }
}

}
