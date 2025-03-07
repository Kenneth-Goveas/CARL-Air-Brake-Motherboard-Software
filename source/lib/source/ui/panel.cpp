#include <cstdint>

#include <string>
#include <vector>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/terminal.hpp>
#include <ui/input.hpp>
#include <ui/output.hpp>
#include <ui/panel.hpp>

namespace panel :: intern {

std::string mod = "panel";

bool fail;
bool eof;

std::vector<terminal::color> color;
std::vector<int> row;
std::vector<int> col;

}

namespace panel {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

void init (void) {
    logging::inf(intern::mod,
        "Configuring terminal"
    );

    terminal::color_default();
    terminal::cursor_pos(1, 1);
    terminal::cursor_hide();
    terminal::erase_screen();
}

void deinit (void) {
    logging::inf(intern::mod,
        "Reverting terminal"
    );

    terminal::color_default();
    terminal::cursor_pos(1, 1);
    terminal::cursor_show();
    terminal::erase_screen();
}

int open (terminal::color color, int row, int col, int hght, int wdth) {
    int id = intern::color.size() + 1;
    std::string space = "";

    logging::inf(intern::mod,
        "Drawing panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Hght: ", hght, ", Wdth: ", wdth
    );

    if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to draw panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return id;
    } else if (hght <= 0 || wdth <= 0) {
        logging::err(intern::mod,
            "Failed to draw panel #", id, " (Invalid dimensions)"
        );
        intern::fail = true;
        return id;
    }

    for (int i = 0; i < wdth; i++) {
        space += ' ';
    }

    terminal::color_back(color);
    for (int i = 0; i < hght; i++) {
        terminal::cursor_pos(row + i, col);
        output::put(space.c_str());
    }

    intern::color.push_back(color);
    intern::row.push_back(row);
    intern::col.push_back(col);

    intern::fail = false;
    return id;
}

void close (void) {
    logging::inf(intern::mod,
        "Erasing panels"
    );

    terminal::color_default();
    terminal::cursor_pos(1, 1);
    terminal::erase_screen();

    intern::color.clear();
    intern::row.clear();
    intern::col.clear();
}

void put (terminal::color color, int id, int row, int col, const char * val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Characters, Value: ", std::string(val)
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::string val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::uint8_t val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, bool val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, int val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, double val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, linalg::ivector val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, linalg::fvector val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, linalg::imatrix val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, linalg::fmatrix val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<std::string> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<std::uint8_t> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<bool> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<int> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<double> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<linalg::ivector> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<linalg::fvector> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<linalg::imatrix> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

void put (terminal::color color, int id, int row, int col, std::vector<linalg::fmatrix> val) {
    logging::inf(intern::mod,
        "Printing output on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix array, Value: ", val
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to print output on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    output::put(val);

    intern::fail = false;
}

template <>
std::string get<std::string> (terminal::color color, int id, int row, int col) {
    std::string val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::string>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::uint8_t get<std::uint8_t> (terminal::color color, int id, int row, int col) {
    std::uint8_t val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::uint8_t>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
bool get<bool> (terminal::color color, int id, int row, int col) {
    bool val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<bool>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int get<int> (terminal::color color, int id, int row, int col) {
    int val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<int>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double get<double> (terminal::color color, int id, int row, int col) {
    double val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<double>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector get<linalg::ivector> (terminal::color color, int id, int row, int col) {
    linalg::ivector val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<linalg::ivector>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector get<linalg::fvector> (terminal::color color, int id, int row, int col) {
    linalg::fvector val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<linalg::fvector>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix get<linalg::imatrix> (terminal::color color, int id, int row, int col) {
    linalg::imatrix val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<linalg::imatrix>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix get<linalg::fmatrix> (terminal::color color, int id, int row, int col) {
    linalg::fmatrix val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<linalg::fmatrix>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<std::string> get<std::vector<std::string>> (terminal::color color, int id, int row, int col) {
    std::vector<std::string> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: String array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<std::string>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<std::uint8_t> get<std::vector<std::uint8_t>> (terminal::color color, int id, int row, int col) {
    std::vector<std::uint8_t> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Byte array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<std::uint8_t>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<bool> get<std::vector<bool>> (terminal::color color, int id, int row, int col) {
    std::vector<bool> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Boolean array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<bool>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<int> get<std::vector<int>> (terminal::color color, int id, int row, int col) {
    std::vector<int> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer scalar array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<int>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<double> get<std::vector<double>> (terminal::color color, int id, int row, int col) {
    std::vector<double> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real scalar array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<double>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::ivector> get<std::vector<linalg::ivector>> (terminal::color color, int id, int row, int col) {
    std::vector<linalg::ivector> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer vector array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<linalg::ivector>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::fvector> get<std::vector<linalg::fvector>> (terminal::color color, int id, int row, int col) {
    std::vector<linalg::fvector> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real vector array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<linalg::fvector>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::imatrix> get<std::vector<linalg::imatrix>> (terminal::color color, int id, int row, int col) {
    std::vector<linalg::imatrix> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Integer matrix array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<linalg::imatrix>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::vector<linalg::fmatrix> get<std::vector<linalg::fmatrix>> (terminal::color color, int id, int row, int col) {
    std::vector<linalg::fmatrix> val;

    logging::inf(intern::mod,
        "Scanning input on panel #", id, ": Color: ", color, ", Row: ", row, ", Col: ", col, ", Type: Real matrix array"
    );

    if (id <= 0 || id > intern::color.size()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (row <= 0 || col <= 0) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(color);
    terminal::color_back(intern::color[id - 1]);
    terminal::cursor_pos(intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1);
    terminal::cursor_show();
    val = input::get<std::vector<linalg::fmatrix>>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan input on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No input received"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    logging::inf(intern::mod,
        "Scanned input on panel #", id, ": Value: ", val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

}
