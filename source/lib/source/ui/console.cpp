#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <math/linalg.hpp>

#include <sys/logging.hpp>

#include <ui/terminal.hpp>
#include <ui/input.hpp>
#include <ui/output.hpp>
#include <ui/console.hpp>

namespace console :: intern {

std::string mod = "console";

bool fail;
bool eof;

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

namespace console {

bool fail (void) {
    return intern::fail;
}

bool eof (void) {
    return intern::eof;
}

void newline (void) {
    logging::inf(intern::mod,
        "Moving to next line"
    );

    terminal::color_default();
    terminal::newline();
    terminal::erase_line();
}

void print (terminal::color clr, int col, bool val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing boolean: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print boolean (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, std::string val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing string: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print string (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, int val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer scalar: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print integer scalar (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, double val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point scalar: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print floating point scalar (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, linalg::ivector val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer vector: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print integer vector (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, linalg::fvector val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point vector: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print floating point vector (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, linalg::imatrix val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing integer matrix: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print integer matrix (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, linalg::fmatrix val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(val);
    logging::inf(intern::mod,
        "Printing floating point matrix: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print floating point matrix (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

void print (terminal::color clr, int col, const char * val) {
    std::string oupt_clr, oupt_val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    oupt_val = intern::conv_val_to_oupt(std::string(val));
    logging::inf(intern::mod,
        "Printing string: Color: ", oupt_clr, ", ",
        "Col: ", col, ", Value: ", oupt_val
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to print string (Invalid coordinates)"
        );
        intern::fail = true;
        return;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    output::print(val);

    intern::fail = false;
}

template <>
bool scan<bool> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    bool val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning boolean: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan boolean (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<bool>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan boolean"
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
        "Scanned boolean: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
std::string scan<std::string> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    std::string val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning string: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan string (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<std::string>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan string"
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
        "Scanned string: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
int scan<int> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    int val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning integer scalar: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan integer scalar (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<int>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan integer scalar"
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
        "Scanned integer scalar: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
double scan<double> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    double val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning floating point scalar: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan floating point scalar (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<double>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan floating point scalar"
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
        "Scanned floating point scalar: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::ivector scan<linalg::ivector> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    linalg::ivector val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning integer vector: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan integer vector (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<linalg::ivector>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan integer vector"
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
        "Scanned integer vector: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fvector scan<linalg::fvector> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    linalg::fvector val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning floating point vector: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan floating point vector (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<linalg::fvector>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan floating point vector"
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
        "Scanned floating point vector: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::imatrix scan<linalg::imatrix> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    linalg::imatrix val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning integer matrix: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan integer matrix (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<linalg::imatrix>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan integer matrix"
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
        "Scanned integer matrix: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

template <>
linalg::fmatrix scan<linalg::fmatrix> (terminal::color clr, int col) {
    std::string oupt_clr, oupt_val;
    linalg::fmatrix val;

    oupt_clr = intern::conv_color_to_oupt(clr);
    logging::inf(intern::mod,
        "Scanning floating point matrix: Color: ", oupt_clr, ", Col: ", col
    );

    if (!(col > 0)) {
        logging::err(intern::mod,
            "Failed to scan floating point matrix (Invalid coordinates)"
        );
        intern::fail = true;
        intern::eof = false;
        return val;
    }

    terminal::color_fore(clr);
    terminal::cursor_col(col);
    val = input::scan<linalg::fmatrix>();

    if (input::fail()) {
        logging::err(intern::mod,
            "Failed to scan floating point matrix"
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
        "Scanned floating point matrix: Value: ", oupt_val
    );

    intern::fail = false;
    intern::eof = false;
    return val;
}

}

namespace console :: intern {

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
