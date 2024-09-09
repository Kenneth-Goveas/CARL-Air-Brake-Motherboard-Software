#include <iomanip>
#include <iostream>
#include <sstream>
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

std::vector<terminal::color> clr;
std::vector<int> row;
std::vector<int> col;

std::string conv_val_to_oupt (bool val);
std::string conv_val_to_oupt (std::string val);
std::string conv_val_to_oupt (int val);
std::string conv_val_to_oupt (double val);
std::string conv_val_to_oupt (linalg::ivector val);
std::string conv_val_to_oupt (linalg::fvector val);
std::string conv_val_to_oupt (linalg::imatrix val);
std::string conv_val_to_oupt (linalg::fmatrix val);

std::string conv_color_to_oupt (terminal::color clr);

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

int draw (terminal::color clr, int row, int col, int hght, int wdth) {
    int id = intern::clr.size() + 1;
    std::string oupt_clr, blank = "";

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Drawing panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Hght: ", hght, ", Wdth: ", wdth
    );

    if (!(row > 0 && col > 0 )) {
        logging::err(intern::mod,
            "Failed to draw panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return id;
    } else if (!(hght > 0 && wdth > 0)) {
        logging::err(intern::mod,
            "Failed to draw panel #", id, " (Invalid dimensions)"
        );
        intern::fail = true;
        return id;
    }

    for (int i = 0; i < wdth; i++) {
        blank += ' ';
    }

    terminal::color_back(clr);
    for (int i = 0; i < hght; i++) {
        terminal::cursor_pos(row + i, col);
        output::print(blank);
    }

    intern::clr.push_back(clr);
    intern::row.push_back(row);
    intern::col.push_back(col);

    intern::fail = false;
    return id;
}

void erase (void) {
    logging::inf(intern::mod,
        "Erasing panels"
    );

    terminal::color_default();
    terminal::cursor_pos(1, 1);
    terminal::erase_screen();

    intern::clr.resize(0);
    intern::row.resize(0);
    intern::col.resize(0);
}

void print (terminal::color clr, int id, int row, int col, bool val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing boolean on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print boolean on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print boolean on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int id, int row, int col, std::string val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing string on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print string on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print string on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int id, int row, int col, int val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer scalar on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print integer scalar on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print integer scalar on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int id, int row, int col, double val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point scalar on panel #", id, ": ",
        "Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print floating point scalar on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print floating point scalar on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (
    terminal::color clr, int id, int row, int col, linalg::ivector val
) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer vector on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print integer vector on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print integer vector on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (
    terminal::color clr, int id, int row, int col, linalg::fvector val
) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point vector on panel #", id, ": ",
        "Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print floating point vector on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print floating point vector on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (
    terminal::color clr, int id, int row, int col, linalg::imatrix val
) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer matrix on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print integer matrix on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print integer matrix on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (
    terminal::color clr, int id, int row, int col, linalg::fmatrix val
) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point matrix on panel #", id, ": ",
        "Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print floating point matrix on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print floating point matrix on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int id, int row, int col, const char * val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(std::string(val));
    logging::inf(intern::mod,
        "Printing string on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col, ", Value: ", oupt_val
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to print string on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        return;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to print string on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    output::print(val);

    intern::fail = false;
}

template <>
bool scan<bool> (terminal::color clr, int id, int row, int col) {
    std::string oupt_clr, oupt_val;
    bool val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning boolean on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan boolean on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan boolean on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<bool>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan boolean on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned boolean on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::string scan<std::string> (terminal::color clr, int id, int row, int col) {
    std::string oupt_clr, oupt_val;
    std::string val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning string on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan string on panel #", id, " (Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan string on panel #", id, " (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<std::string>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan string on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned string on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int scan<int> (terminal::color clr, int id, int row, int col) {
    std::string oupt_clr, oupt_val;
    int val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning integer scalar on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan integer scalar on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan integer scalar on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<int>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan integer scalar on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned integer scalar on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double scan<double> (terminal::color clr, int id, int row, int col) {
    std::string oupt_clr, oupt_val;
    double val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning floating point scalar on panel #", id, ": ",
        "Color: ", oupt_clr, ", Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan floating point scalar on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan floating point scalar on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<double>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan floating point scalar on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned floating point scalar on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector scan<linalg::ivector> (
    terminal::color clr, int id, int row, int col
) {
    std::string oupt_clr, oupt_val;
    linalg::ivector val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning integer vector on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan integer vector on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan integer vector on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<linalg::ivector>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan integer vector on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned integer vector on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector scan<linalg::fvector> (
    terminal::color clr, int id, int row, int col
) {
    std::string oupt_clr, oupt_val;
    linalg::fvector val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning floating point vector on panel #", id, ": ",
        "Color: ", oupt_clr, ", Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan floating point vector on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan floating point vector on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<linalg::fvector>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan floating point vector on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned floating point vector on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix scan<linalg::imatrix> (
    terminal::color clr, int id, int row, int col
) {
    std::string oupt_clr, oupt_val;
    linalg::imatrix val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning integer matrix on panel #", id, ": Color: ", oupt_clr, ", ",
        "Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan integer matrix on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan integer matrix on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<linalg::imatrix>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan integer matrix on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned integer matrix on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix scan<linalg::fmatrix> (
    terminal::color clr, int id, int row, int col
) {
    std::string oupt_clr, oupt_val;
    linalg::fmatrix val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning floating point matrix on panel #", id, ": ",
        "Color: ", oupt_clr, ", Row: ", row, ", Col: ", col
    );

    if (!(id > 0 && id <= intern::clr.size())) {
        logging::err(intern::mod,
            "Failed to scan floating point matrix on panel #", id, " ",
            "(Panel not found)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    } else if (!(row > 0 && col > 0)) {
        logging::err(intern::mod,
            "Failed to scan floating point matrix on panel #", id, " ",
            "(Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::color_back(intern::clr[id - 1]);
    terminal::cursor_pos(
        intern::row[id - 1] + row - 1, intern::col[id - 1] + col - 1
    );
    terminal::cursor_show();
    val = input::scan<linalg::fmatrix>();
    terminal::cursor_hide();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan floating point matrix on panel #", id
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    if (input::eof()) {
        logging::wrn(intern::mod,
            "No user input supplied"
        );
        intern::fail = false;
        intern::eof = true;
        return val;
    }

    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Scanned floating point matrix on panel #", id, ": Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

}

namespace panel :: intern {

std::string conv_val_to_oupt (bool val) {
    std::string oupt;
    if (val) {
        oupt = "True";
    } else {
        oupt = "False";
    }
    return oupt;
}

std::string conv_val_to_oupt (std::string val) {
    std::string oupt;
    oupt = std::string("“") + val + std::string("”");
    return oupt;
}

std::string conv_val_to_oupt (int val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (double val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << std::showpos << std::scientific << std::setprecision(2) << val;
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::ivector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fvector val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::dim(val); i++) {
        ostr << std::showpos << std::scientific << std::setprecision(2)
             << val[i];
        if (i < linalg::dim(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::imatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_val_to_oupt (linalg::fmatrix val) {
    std::ostringstream ostr;
    std::string oupt;

    ostr << "[";
    for (int i = 0; i < linalg::rows(val); i++) {
        ostr << "[";
        for (int j = 0; j < linalg::cols(val); j++) {
            ostr << std::showpos << std::scientific << std::setprecision(2)
                 << val[i][j];
            if (j < linalg::cols(val) - 1) {
                ostr << ", ";
            }
        }
        ostr << "]";
        if (i < linalg::rows(val) - 1) {
            ostr << ", ";
        }
    }
    ostr << "]";
    oupt = ostr.str();

    return oupt;
}

std::string conv_color_to_oupt (terminal::color clr) {
    switch (clr) {
        case terminal::black:
            return std::string("Black");
        case terminal::darkred:
            return std::string("Dark red");
        case terminal::darkgreen:
            return std::string("Dark green");
        case terminal::darkyellow:
            return std::string("Dark yellow");
        case terminal::darkblue:
            return std::string("Dark blue");
        case terminal::darkmagenta:
            return std::string("Dark magenta");
        case terminal::darkcyan:
            return std::string("Dark cyan");
        case terminal::lightgray:
            return std::string("Light gray");
        case terminal::darkgray:
            return std::string("Dark gray");
        case terminal::lightred:
            return std::string("Light red");
        case terminal::lightgreen:
            return std::string("Light green");
        case terminal::lightyellow:
            return std::string("Light yellow");
        case terminal::lightblue:
            return std::string("Light blue");
        case terminal::lightmagenta:
            return std::string("Light magenta");
        case terminal::lightcyan:
            return std::string("Light cyan");
        case terminal::white:
            return std::string("White");
        default:
            return std::string("");
    }
}

}
