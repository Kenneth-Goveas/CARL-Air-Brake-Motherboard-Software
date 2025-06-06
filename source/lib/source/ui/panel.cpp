#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>
#include <sys/logging.hpp>
#include <ui/terminal.hpp>
#include <ui/input.hpp>
#include <ui/output.hpp>
#include <ui/panel.hpp>

namespace panel {

int tracker::count = 0;

int tracker::instantiate (void) {
    tracker::count++;
    return tracker::count;
}

instance::instance (
    terminal::instance & terminal, input::instance & input, output::instance & output,
    terminal::color color, int row, int col, int hgt, int wdt
) : logging("panel"), init(false), id(instance::instantiate()),
    terminal(&terminal), input(&input), output(&output), color(color), row(row), col(col), hgt(hgt), wdt(wdt) {

    std::string space;

    this->logging.inf(
        "Initializing instance #", this->id, ": ",
        "Color: ", this->color, ", Row: ", this->row, ", Col: ", this->col, ", Hgt: ", this->hgt, ", Wdt: ", this->wdt
    );

    this->logging.inf("Drawing panel #", this->id);

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to draw panel #", this->id, " (Invalid coordinates)");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    if (hgt <= 0 || wdt <= 0) {
        this->logging.err("Failed to draw panel #", this->id, " (Invalid dimensions)");
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    for (int i = 0; i < this->wdt; i++) {
        space.push_back(' ');
    }

    try {
        this->terminal->color_back(this->color);
        for (int i = 0; i < this->hgt; i++) {
            this->terminal->cursor_move(this->row + i, this->col);
            this->output->put(space.c_str());
        }
    } catch (...) {
        this->logging.err("Failed to draw panel #", this->id);
        this->logging.err("Failed to initialize instance #", this->id);
        throw except_ctor::fail;
    }

    this->init = true;
}

void instance::put (terminal::color color, int row, int col, const char * val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Characters, Value: ", std::string(val)
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::string & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, std::uint8_t val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, bool val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, int val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, double val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const linalg::ivector & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const linalg::fvector & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const linalg::imatrix & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const linalg::fmatrix & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<std::string> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<std::uint8_t> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<bool> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<int> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<double> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<linalg::ivector> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<linalg::fvector> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<linalg::imatrix> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

void instance::put (terminal::color color, int row, int col, const std::vector<linalg::fmatrix> & val) {
    this->logging.inf(
        "Printing output on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix array, Value: ", val
    );

    if (!this->init) {
        this->logging.err("Failed to print output on panel #", this->id, " (Instance not initialized)");
        throw except_put::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to print output on panel #", this->id, " (Invalid coordinates)");
        throw except_put::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->output->put(val);
    } catch (...) {
        this->logging.err("Failed to print output on panel #", this->id);
        throw except_put::fail;
    }
}

template <>
std::string instance::get<std::string> (terminal::color color, int row, int col) {
    std::string val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::string>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::uint8_t instance::get<std::uint8_t> (terminal::color color, int row, int col) {
    std::uint8_t val = 0;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::uint8_t>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
bool instance::get<bool> (terminal::color color, int row, int col) {
    bool val = 0;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<bool>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
int instance::get<int> (terminal::color color, int row, int col) {
    int val = 0;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<int>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
double instance::get<double> (terminal::color color, int row, int col) {
    double val = 0;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<double>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
linalg::ivector instance::get<linalg::ivector> (terminal::color color, int row, int col) {
    linalg::ivector val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<linalg::ivector>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
linalg::fvector instance::get<linalg::fvector> (terminal::color color, int row, int col) {
    linalg::fvector val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<linalg::fvector>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
linalg::imatrix instance::get<linalg::imatrix> (terminal::color color, int row, int col) {
    linalg::imatrix val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<linalg::imatrix>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
linalg::fmatrix instance::get<linalg::fmatrix> (terminal::color color, int row, int col) {
    linalg::fmatrix val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<linalg::fmatrix>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<std::string> instance::get<std::vector<std::string>> (terminal::color color, int row, int col) {
    std::vector<std::string> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<std::string>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<std::uint8_t> instance::get<std::vector<std::uint8_t>> (terminal::color color, int row, int col) {
    std::vector<std::uint8_t> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<std::uint8_t>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<bool> instance::get<std::vector<bool>> (terminal::color color, int row, int col) {
    std::vector<bool> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<bool>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<int> instance::get<std::vector<int>> (terminal::color color, int row, int col) {
    std::vector<int> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<int>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<double> instance::get<std::vector<double>> (terminal::color color, int row, int col) {
    std::vector<double> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<double>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<linalg::ivector> instance::get<std::vector<linalg::ivector>> (terminal::color color, int row, int col) {
    std::vector<linalg::ivector> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<linalg::ivector>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<linalg::fvector> instance::get<std::vector<linalg::fvector>> (terminal::color color, int row, int col) {
    std::vector<linalg::fvector> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<linalg::fvector>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<linalg::imatrix> instance::get<std::vector<linalg::imatrix>> (terminal::color color, int row, int col) {
    std::vector<linalg::imatrix> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<linalg::imatrix>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

template <>
std::vector<linalg::fmatrix> instance::get<std::vector<linalg::fmatrix>> (terminal::color color, int row, int col) {
    std::vector<linalg::fmatrix> val;

    this->logging.inf(
        "Scanning input on panel #", this->id, ": ",
        "Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix array"
    );

    if (!this->init) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Instance not initialized)");
        throw except_get::fail;
    }

    if (row <= 0 || col <= 0) {
        this->logging.err("Failed to scan input on panel #", this->id, " (Invalid coordinates)");
        throw except_get::fail;
    }

    try {
        this->terminal->color_fore(color);
        this->terminal->color_back(this->color);
        this->terminal->cursor_move(this->row + row - 1, this->col + col - 1);
        this->terminal->cursor_show();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    try {
        val = this->input->get<std::vector<linalg::fmatrix>>();
    } catch (input::instance::except_get except) {
        try {
            this->terminal->cursor_hide();
        } catch (...) {
            this->logging.err("Failed to scan input on panel #", this->id);
            throw except_get::fail;
        }
        switch (except) {
            case input::instance::except_get::fail:
                this->logging.err("Failed to scan input on panel #", this->id);
                throw except_get::fail;
            case input::instance::except_get::eof:
                this->logging.wrn("No input received on panel #", this->id);
                throw except_get::eof;
        }
    }

    try {
        this->terminal->cursor_hide();
    } catch (...) {
        this->logging.err("Failed to scan input on panel #", this->id);
        throw except_get::fail;
    }

    this->logging.inf("Scanned input on panel #", this->id, ": Value: ", val);

    return val;
}

void instance::redraw (void) {
    std::string space;

    this->logging.inf("Redrawing panel #", this->id);

    if (!this->init) {
        this->logging.err("Failed to redraw panel #", this->id, " (Instance not initialized)");
        throw except_redraw::fail;
    }

    for (int i = 0; i < this->wdt; i++) {
        space.push_back(' ');
    }

    try {
        this->terminal->color_back(this->color);
        for (int i = 0; i < this->hgt; i++) {
            this->terminal->cursor_move(this->row + i, this->col);
            this->output->put(space.c_str());
        }
    } catch (...) {
        this->logging.err("Failed to redraw panel #", this->id);
        throw except_redraw::fail;
    }
}

}
